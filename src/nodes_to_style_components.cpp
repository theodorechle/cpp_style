#include "nodes_to_style_components.hpp"
#include "style_component.hpp"

namespace style {

    StyleComponentType tokenTypeToStyleComponentType(Token token) {
        switch (token) {
        case Token::StarWildcard:
            return StyleComponentType::StarWildcard;
        case Token::ElementName:
            return StyleComponentType::ElementName;
        case Token::Class:
            return StyleComponentType::Class;
        case Token::Modifier:
            return StyleComponentType::Modifier;
        case Token::Identifier:
            return StyleComponentType::Identifier;
        default:
            return StyleComponentType::Null;
        }
    }

    StyleValueType tokenTypeToStyleValueType(Token token) {
        switch (token) {
        case Token::Int:
            return StyleValueType::Int;
        case Token::Float:
            return StyleValueType::Float;
        case Token::Bool:
            return StyleValueType::Bool;
        case Token::String:
            return StyleValueType::String;
        case Token::Tuple:
            return StyleValueType::Tuple;
        case Token::Function:
            return StyleValueType::Function;
        case Token::Unit:
            return StyleValueType::Unit;
        case Token::Hex:
            return StyleValueType::Hex;
        case Token::EnumValue:
            return StyleValueType::EnumValue;
        default:
            return StyleValueType::Null;
        }
    }

    StyleRelation tokenTypeToStyleRelation(Token token) {
        switch (token) {
        case Token::DirectParent:
            return StyleRelation::DirectParent;
        case Token::AnyParent:
            return StyleRelation::AnyParent;
        case Token::SameElement:
            return StyleRelation::SameElement;
        default:
            return StyleRelation::Null;
        }
    }

    DeserializationNode *NodesToStyleComponents::importStyle(const std::string &fileName) {
        // FIXME: duplicate with style_deserializer
        DeserializationNode *tokens = nullptr;
        DeserializationNode *result = nullptr;
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
        buffer << file.rdbuf();
        try {
            tokens = Lexer().lexe(buffer.str(), _config);
            result = Parser().parse(tokens);
            delete tokens;
            return result;
        }
        catch (const ParserException &) {
            delete tokens;
            throw;
        }
    }

    DeserializationNode *NodesToStyleComponents::joinStyleDeclarations(DeserializationNode *firstDeclarations,
                                                                       DeserializationNode *secondDeclarations) {
        DeserializationNode *newDeclarations = new DeserializationNode(Token::NullRoot);
        DeserializationNode *actualDeclaration;
        DeserializationNode *secondDeclarationsIt;

        while (firstDeclarations != nullptr) {
            secondDeclarationsIt = secondDeclarations;
            while (secondDeclarationsIt != nullptr) {
                actualDeclaration = newDeclarations->appendNext(firstDeclarations->copyNodeWithChilds());
                if (tokenTypeToStyleRelation(secondDeclarationsIt->child()->token()) == StyleRelation::Null)
                    actualDeclaration->addChild(new DeserializationNode(Token::AnyParent));
                if (tokenTypeToStyleRelation(secondDeclarationsIt->child()->token()) == StyleRelation::SameElement)
                    secondDeclarationsIt->deleteSpecificChild(secondDeclarationsIt->child());
                actualDeclaration->addChild(secondDeclarationsIt->child()->copyNodeWithChildsAndNexts());
                secondDeclarationsIt = secondDeclarationsIt->next();
            }
            firstDeclarations = firstDeclarations->next();
        }

        DeserializationNode *root = newDeclarations;
        newDeclarations = newDeclarations->next();
        root->next(nullptr);
        delete root;
        return newDeclarations;
    }

    void NodesToStyleComponents::moveNestedBlocksToRoot(DeserializationNode *style) {
        DeserializationNode *blockDeclarations = style->child();
        DeserializationNode *content = blockDeclarations->next()->child();
        DeserializationNode *nextDeclaration;

        while (content != nullptr) {
            if (content->token() == Token::StyleBlock) {
                content->child()->replaceChild(content->child()->child(),
                                               joinStyleDeclarations(blockDeclarations->child(), content->child()->child()));
                nextDeclaration = content->next();
                content->parent()->removeSpecificChild(content);
                content->next(style->next());
                style->next(content);
                content->setParent(style->parent());
                style = content;
                content = nextDeclaration;
                continue;
            }
            content = content->next();
        }
    }

    void NodesToStyleComponents::flattenStyle(DeserializationNode *style) {
        if (style == nullptr) return;
        style = style->child();
        while (style != nullptr) {
            if (style->token() == Token::StyleBlock) moveNestedBlocksToRoot(style);
            else if (style->token() == Token::Import) {
                DeserializationNode *importedStyle = importStyle(style->value()); // refactor with StyleManager
                if (importedStyle != nullptr) {
                    importedStyle->addChild(style->next());
                    style->next(importedStyle->child());
                    importedStyle->setChild(nullptr);
                    delete importedStyle;
                }
            }
            style = style->next();
        }
    }

    bool NodesToStyleComponents::ruleNodesValid(const DeserializationNode *ruleNode, const config::ConfigRuleNode *configNode) {
        if (ruleNode == nullptr && configNode == nullptr) return true;
        if (ruleNode == nullptr || configNode == nullptr) return false;
        if (ruleNode->token() != configNode->token()) return false;
        if (configNode->token() == Token::EnumValue) {
            const std::set<std::string> &allowedValues = static_cast<const config::ConfigRuleNodeEnum *>(configNode)->allowedValues();
            return allowedValues.find(ruleNode->value()) != allowedValues.cend();
        }
        return true;
    }

    bool NodesToStyleComponents::ruleValid(const DeserializationNode *rule) {
        const DeserializationNode *ruleName = rule->child();
        const DeserializationNode *ruleValue = ruleName->next();
        std::unordered_map<std::string, std::vector<const config::ConfigRuleNode *>>::const_iterator configRules =
            _config->rules.find(ruleName->value());
        if (configRules == _config->rules.cend()) return false;
        for (const config::ConfigRuleNode *configRule : configRules->second) {
            if (ruleNodesValid(ruleValue, configRule)) return true;
        }
        return false;
    }

    void NodesToStyleComponents::filterRulesWithConfiguration(DeserializationNode *style) {
        if (style == nullptr) return;
        DeserializationNode *block = style->child();
        DeserializationNode *declaration;
        DeserializationNode *rule;
        DeserializationNode *nextRule;
        DeserializationNode *next;

        while (block) {
            declaration = block->getLastChild();
            if (!declaration) {
                block = block->next();
                continue;
            }
            rule = declaration->child();
            while (rule) {
                nextRule = rule->next();
                if (rule->token() == Token::Assignment && !ruleValid(rule)) {
#ifdef DEBUG
                    std::cerr << "invalid rule:\n";
                    rule->debugDisplay();
                    std::unordered_map<std::string, std::vector<const config::ConfigRuleNode *>>::const_iterator configRules =
                        _config->rules.find(rule->child()->value());
                    if (configRules != _config->rules.cend()) {
                        std::cerr << "available config rules:\n";
                        for (const config::ConfigRuleNode *configRule : configRules->second) {
                            configRule->debugDisplay();
                        }
                    }

#endif
                    declaration->removeSpecificChild(rule);
                    delete rule;
                }
                rule = nextRule;
            }
            if (declaration->nbChilds() == 0) {
                next = block->next();
                style->removeSpecificChild(block);
                delete block;
                block = next;
            }
            else block = block->next();
        }
    }

    std::list<StyleComponentDataList *> *NodesToStyleComponents::convertStyleComponents() {
        std::list<StyleComponentDataList *> *styleComponentsLists;
        StyleComponentDataList *requiredStyleComponents;
        DeserializationNode *declaration;
        DeserializationNode *declarationPart;
        Token nextDeclarationToken;
        StyleComponentType styleComponentType;
        StyleRelation styleRelationToken;
        std::string currentValue;

        if (tree == nullptr || tree->token() != Token::BlockSelectors)
            return nullptr; // TODO: it can't happen if previous steps passed (I think there is a lot of checks who are useless)

        styleComponentsLists = new std::list<StyleComponentDataList *>();
        declaration = tree->child();
        // loop through declarations
        while (declaration != nullptr) {
            if (declaration->token() != Token::Selector) { // invalid block declaration
                delete styleComponentsLists;               // TODO: how could this even happen?
                return nullptr;
            }
            requiredStyleComponents = new StyleComponentDataList();
            declarationPart = declaration->child();
            // loop through each value in a declaration
            while (declarationPart != nullptr) {
                styleComponentType = tokenTypeToStyleComponentType(declarationPart->token());
                currentValue = declarationPart->value();
                declarationPart = declarationPart->next();

                // if invalid declaration, skip it
                // TODO: how can it happen too?
                if (styleComponentType == StyleComponentType::Null) continue;

                if (declarationPart == nullptr) styleRelationToken = StyleRelation::SameElement; // last value
                else {
                    nextDeclarationToken = declarationPart->token();
                    if (tokenTypeToStyleComponentType(nextDeclarationToken) != StyleComponentType::Null) {
                        styleRelationToken = StyleRelation::SameElement;
                    }
                    else {
                        styleRelationToken = tokenTypeToStyleRelation(nextDeclarationToken);
                        if (styleRelationToken != StyleRelation::Null) declarationPart = declarationPart->next();
                    }
                }
                if (styleRelationToken != StyleRelation::Null) {
                    requiredStyleComponents->push_back(std::pair(StyleComponentData(currentValue, styleComponentType), styleRelationToken));
                }
            }
            styleComponentsLists->push_back(requiredStyleComponents);
            declaration = declaration->next();
        }
        return styleComponentsLists;
    }

    StyleValue *NodesToStyleComponents::convertStyleNodeToStyleValue(DeserializationNode *node) {
        if (node == nullptr) return nullptr;
        StyleValueType type;
        DeserializationNode *next;
        StyleValue *styleValue;
        StyleValue *tmpStyleValue;
        StyleValue *styleNext;

        type = tokenTypeToStyleValueType(node->token());
        if (type == StyleValueType::Null) return nullptr;

        styleValue = new StyleValue();
        styleValue->setValue(node->value());
        styleValue->setType(type);

        styleValue->setChild(convertStyleNodeToStyleValue(node->child()));

        next = node->next();
        tmpStyleValue = styleValue;
        while (next != nullptr) {
            styleNext = convertStyleNodeToStyleValue(next);
            if (styleNext == nullptr) break;
            tmpStyleValue->setNext(styleNext);
            tmpStyleValue = styleNext;
            next = next->next();
        }
        return styleValue;
    }

    StyleValuesMap *NodesToStyleComponents::convertAppliedStyle(int fileNumber, int *ruleNumber) {
        StyleValuesMap *appliedStyleMap;
        StyleValue *styleValue;
        std::string ruleName;
        DeserializationNode *rule;
        DeserializationNode *oldTree;
        DeserializationNode *ruleNameNode;
        Token token;
        if (tree == nullptr || tree->token() != Token::BlockDeclarations) return nullptr;

        appliedStyleMap = new StyleValuesMap();
        rule = tree->child();
        while (rule != nullptr) {
            token = rule->token();
            if (token == Token::Assignment) {
                ruleNameNode = rule->child();
                if (ruleNameNode == nullptr || ruleNameNode->token() != Token::RuleName) {
                    rule = rule->next();
                    continue;
                }
                ruleName = ruleNameNode->value();
                ruleNameNode = ruleNameNode->next();
                if (!isNodeNull(ruleNameNode)) {
                    styleValue = convertStyleNodeToStyleValue(ruleNameNode);
                    if (styleValue != nullptr) {
                        (*appliedStyleMap)[ruleName] = StyleRule(styleValue->copy(), true, 0, fileNumber, *ruleNumber);
                        (*ruleNumber)++;
                    }
                }
            }
            else if (token == Token::StyleBlock) {
                oldTree = tree;
                tree = rule;
                convertStyleBlock(fileNumber, ruleNumber);
                tree = oldTree;
            }
            // if other token type, just don't use it
            rule = rule->next();
        }

        return appliedStyleMap;
    }

    std::list<StyleBlock *> *
    NodesToStyleComponents::createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt,
                                                  StyleComponentDataList *components, StyleValuesMap *appliedStyleMap) {
        if (components == nullptr) return nullptr;
        std::list<StyleBlock *> *styleComponentList = new std::list<StyleBlock *>();
        std::list<StyleBlock *> *tmpStyleComponentList;
        StyleComponentDataList::const_iterator componentsIt;
        if (std::next(componentsListIt) == requiredStyleComponentsLists.cend()) { // if at end of the declaration list
            for (StyleComponentDataList *componentsDataList : **componentsListIt) {
                componentsIt = std::prev(components->end());
                std::copy(componentsDataList->begin(), componentsDataList->end(), std::back_inserter(*components));
                int specificity = computeRuleSpecifity(components);
                for (std::pair<const std::string, StyleRule> &rule : *appliedStyleMap) {
                    rule.second.specificity = specificity;
                }
                styleComponentList->push_back(new StyleBlock(*components, *appliedStyleMap));
                if (componentsIt == components->end()) components->clear();
                else components->erase(componentsIt, components->end());
            }
        }
        else {
            for (StyleComponentDataList *componentsList : **componentsListIt) {
                componentsIt = std::prev(components->end());
                std::copy(componentsList->begin(), componentsList->end(), std::back_inserter(*components));
                tmpStyleComponentList = createStyleComponents(std::next(componentsListIt), components, appliedStyleMap);
                if (tmpStyleComponentList != nullptr) {
                    styleComponentList->splice(styleComponentList->end(), *tmpStyleComponentList);
                    delete tmpStyleComponentList;
                }
                components->erase(componentsIt, components->end());
            }
        }
        return styleComponentList;
    }

    int NodesToStyleComponents::computeRuleSpecifity(StyleComponentDataList *ruleComponents) {
        int specificity = 0;
        for (std::pair<StyleComponentData, StyleRelation> component : *ruleComponents) {
            switch (component.first.second) {
            case StyleComponentType::Identifier:
                specificity += 100;
                break;
            case StyleComponentType::Modifier:
            case StyleComponentType::Class:
                specificity += 10;
                break;
            case StyleComponentType::ElementName:
                specificity += 1;
                break;
            default:
                break;
            }
        }
        return specificity;
    }

    void NodesToStyleComponents::convertStyleBlock(int fileNumber, int *ruleNumber) {
        std::list<StyleComponentDataList *> *styleComponentsLists;
        if (tree == nullptr || tree->token() != Token::StyleBlock) return;
        tree = tree->child();

        styleComponentsLists = convertStyleComponents();
        if (styleComponentsLists == nullptr || styleComponentsLists->empty()) {
            delete styleComponentsLists;
            return;
        }

        tree = tree->next();
        StyleValuesMap *appliedStyleMap = convertAppliedStyle(fileNumber, ruleNumber);
        tree = tree->parent();
        if (appliedStyleMap == nullptr || appliedStyleMap->empty()) { // should be useless since there is now removeEmptyBlocks method
            delete styleComponentsLists;
            delete appliedStyleMap;
            return;
        }

        requiredStyleComponentsLists.push_back(styleComponentsLists); // TODO: I think there is one list who is useless
        StyleComponentDataList components = StyleComponentDataList();
        std::list<StyleBlock *> *finalStyleComponents = createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyleMap);

        delete appliedStyleMap;
        for (StyleComponentDataList *componentDataList : *(requiredStyleComponentsLists.back())) {
            delete componentDataList;
        }
        delete requiredStyleComponentsLists.back();
        requiredStyleComponentsLists.pop_back();
        if (finalStyleComponents != nullptr) {
            styleDefinitions->splice(styleDefinitions->end(), *finalStyleComponents);
            delete finalStyleComponents;
        }
    }

    std::list<StyleBlock *> *NodesToStyleComponents::convert(DeserializationNode *styleTree, int fileNumber, int *ruleNumber) {
        *ruleNumber = 0;
        if (styleTree->token() != Token::NullRoot) return nullptr;

        flattenStyle(styleTree);
#ifdef DEBUG
        std::cerr << "flattened style\n";
        styleTree->debugDisplay(std::cerr);
#endif
        filterRulesWithConfiguration(styleTree);
#ifdef DEBUG
        std::cerr << "filtered style\n";
        styleTree->debugDisplay(std::cerr);
#endif
        tree = styleTree->child();

        styleDefinitions = new std::list<StyleBlock *>();

        while (tree != nullptr) {
            convertStyleBlock(fileNumber, ruleNumber);
            tree = tree->next();
        }

        requiredStyleComponentsLists.clear();

        return styleDefinitions;
    }

} // namespace style
