#include "style_nodes_manager.hpp"

namespace style {
    namespace elementStyle {
        namespace manager {

            int StyleNodesManager::findFile(const std::string &fileName) {
                for (std::pair<int, std::pair<std::string, int>> file : files) {
                    if (file.second.first == fileName) return file.first;
                }
                return -1;
            }

            void StyleNodesManager::updateRulesPrioritiesInElements(int oldFileNumber, int newFileNumber, elementStyle::StyleNode *element) {
                elementStyle::StyleNode *child;
                element->updateStylePriorityFromFile(oldFileNumber, newFileNumber);
                child = element->child();
                while (child != nullptr) {
                    updateRulesPrioritiesInElements(oldFileNumber, newFileNumber, child);
                    child = child->next();
                }
            }

            void StyleNodesManager::updateRulesPriorities(int fileNumber) {
                std::unordered_map<int, std::pair<std::string, int>>::node_type node;
                if (elements != nullptr) {
                    updateRulesPrioritiesInElements(fileNumber, fileCount, elements);
                    node = files.extract(fileNumber);
                    node.key() = fileCount;
                    files.insert(std::move(node));
                    fileCount++;
                }
            }

            void StyleNodesManager::applySpecificStyleToElement(std::list<StyleBlock *> specificStyle,
                                                                elementStyle::StyleNode *elementStyle, bool recursive) {
                if (elementStyle == nullptr) return;
                elementStyle::StyleNode *actualElementStyle = elementStyle;
                const StyleValuesMap *styleMap;
                AppliedStyleMap elementStyleMap;
                std::string modifier = "";
                actualElementStyle->clear();
                for (StyleBlock *styleComponent : specificStyle) {
                    modifier = "";
                    const StyleComponentDataList *componentsList = styleComponent->getComponentsList();

                    if (!elementSelectorsCompatibles(actualElementStyle, componentsList)) continue;

                    styleMap = styleComponent->getStyleMap();
                    elementStyleMap = AppliedStyleMap();

                    if (componentsList->back().first.second == StyleComponentType::Modifier) {
                        modifier = componentsList->back().first.first;
                        actualElementStyle->addModifier(modifier);
                    }

                    for (std::pair<std::string, StyleRule> styleRule : *styleMap) {
                        elementStyleMap[styleRule.first] = {StyleRule{
                            styleRule.second.value->copy(), modifier.empty(), // disable if affected by modifier should be done by elementStyle
                            styleRule.second.specificity, styleRule.second.fileNumber, styleRule.second.ruleNumber}};
                        if (!modifier.empty()) {
                            actualElementStyle->addRuleAffectedByModifier(styleRule.second.fileNumber, styleRule.second.ruleNumber, modifier);
                        }
                    }
                    actualElementStyle->addStyle(elementStyleMap);
                }

                if (recursive) {
                    actualElementStyle = actualElementStyle->child();
                    while (actualElementStyle != nullptr) {
                        applySpecificStyleToElement(specificStyle, actualElementStyle, recursive);
                        actualElementStyle = actualElementStyle->next();
                    }
                }
            }

            StyleNodesManager::~StyleNodesManager() {
                for (StyleBlock *styleBlock : style) {
                    delete styleBlock;
                }
            }

            void StyleNodesManager::setFontsPath(const std::string &path) {
                if (path[path.size() - 1] != '/') fontsPath = path + '/';
                else fontsPath = path;
            }

            const std::string &StyleNodesManager::getFontsPath() const { return fontsPath; }

            int StyleNodesManager::addStyleFile(const std::string &fileName) {
#ifdef DEBUG
                std::cerr << "Adding file '" << fileName << "'\n";
#endif
                int fileNumber = findFile(fileName); //  TODO: allow force reload
                if (fileNumber != -1) removeStyle(fileNumber);
                std::ifstream file(fileName);
                std::stringstream buffer;
                if (!file.is_open()) {
                    std::cerr << "File '" << fileName << "' couldn't be opened\n";
                    return -1;
                }
                buffer << file.rdbuf();
                fileNumber = addStyle(buffer.str());
                files[fileNumber].first = fileName;
                return fileNumber;
            }

            int StyleNodesManager::addStyle(const std::string &styleFileContent) {
                int ruleNumber;
                std::list<StyleBlock *> *fileRules;
                fileRules = StyleDeserializer::deserialize(styleFileContent, fileCount, &ruleNumber, config);
                if (fileRules == nullptr || fileRules->empty()) return -1;
                style.splice(style.end(), *fileRules);
                applySpecificStyleToElement(style, elements, true);
                delete fileRules;
                files[fileCount] = std::pair<std::string, int>("", ruleNumber);
                fileCount++;
                return fileCount - 1;
            }

            void StyleNodesManager::removeStyleInElements(int fileNumber, elementStyle::StyleNode *element) {
#ifdef DEBUG
                std::cerr << "Removing style from element\n";
#endif
                elementStyle::StyleNode *child;
                element->deleteStyleFromFile(fileNumber);
                child = element->child();
                while (child != nullptr) {
                    removeStyleInElements(fileNumber, child);
                    child = child->next();
                }
            }

            void StyleNodesManager::removeStyle(int fileNumber) {
                for (std::pair<int, std::pair<std::string, int>> file : files) {
                }
                if (elements != nullptr && files.find(fileNumber) != files.cend()) {
                    removeStyleInElements(fileNumber, elements);
                    for (StyleBlock *block : style) {
                        StyleValuesMap *styleMap = block->getStyleMap();
                        for (StyleValuesMap::iterator ruleIt = styleMap->begin(); ruleIt != styleMap->end();) {
                            if (ruleIt->second.fileNumber == fileNumber) ruleIt = styleMap->erase(ruleIt);
                            else ruleIt++;
                        }
                    }
                    files.erase(fileNumber);
                }
            }

            bool StyleNodesManager::elementSelectorsCompatibles(elementStyle::StyleNode *elementStyle,
                                                                const StyleComponentDataList *componentsList) {
                if (elementStyle == nullptr || componentsList == nullptr) return false;
                StyleComponentDataList::const_reverse_iterator componentDataIt = componentsList->crbegin();
                if (componentDataIt->first.second == StyleComponentType::Modifier)
                    componentDataIt++; // TODO: rework to put modifiers inside the declaration, not only at the end
                return elementSelectorsCompatiblesLoop(componentDataIt, componentsList->crend(), elementStyle);
            }

            bool StyleNodesManager::elementSelectorsCompatiblesLoop(StyleComponentDataList::const_reverse_iterator componentDataIt,
                                                                    StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                                                    elementStyle::StyleNode *elementStyle) {
                elementStyle::StyleNode *currentStyle = elementStyle;
                const std::set<StyleComponentData> *elementSelectors;

                for (StyleComponentDataList::const_reverse_iterator it = componentDataIt; it != componentDataListEndIt; it++) {
                    if (it->first.second == StyleComponentType::StarWildcard) {
                        continue;
                    }
                    switch (it->second) {
                    case StyleRelation::SameElement:
                        elementSelectors = currentStyle->getSelectors();
                        if (elementSelectors->find(it->first) == elementSelectors->cend()) return false;
                        break;
                    case StyleRelation::DirectParent:
                        currentStyle = currentStyle->parent();
                        if (!currentStyle) return false;

                        elementSelectors = currentStyle->getSelectors();
                        if (elementSelectors->find(it->first) == elementSelectors->cend()) return false;
                        break;
                    case StyleRelation::AnyParent:
                        while (currentStyle) {
                            currentStyle = currentStyle->parent();
                            if (!currentStyle) return false;

                            elementSelectors = currentStyle->getSelectors();
                            if (elementSelectors->find(it->first) == elementSelectors->cend()) continue;
                            StyleComponentDataList::const_reverse_iterator nextIt = std::next(it);
                            if (nextIt == componentDataListEndIt || elementSelectorsCompatiblesLoop(nextIt, componentDataListEndIt, currentStyle)) {
                                return true;
                            }
                        }
                        return false;
                    default:
                        return false;
                    }
                }
                return true;
            }

            void StyleNodesManager::applyStyleToElement(elementStyle::StyleNode *elementStyle, bool recursive) {
                applySpecificStyleToElement(style, elementStyle, recursive);
            }

            void StyleNodesManager::addElementStyle(elementStyle::StyleNode *elementStyle) {
                if (elements == nullptr) setParentElementStyle(elementStyle);
                applyStyleToElement(elementStyle, true);
            }

        } // namespace manager
    } // namespace elementStyle
} // namespace style
