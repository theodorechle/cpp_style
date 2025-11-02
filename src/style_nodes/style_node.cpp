#include "style_node.hpp"

namespace style {
    namespace elementStyle {

        bool StyleNode::compareRulesPriorityDescending(StyleRule rule1, StyleRule rule2) {
            if (rule1.specificity > rule2.specificity) return true;
            if (rule1.specificity == rule2.specificity) {
                if (rule1.fileNumber > rule2.fileNumber) return true;
                if (rule1.fileNumber == rule2.fileNumber) {
                    if (rule1.ruleNumber > rule2.ruleNumber) return true;
                }
            }
            return false;
        }

        bool StyleNode::compareRulesPriorityAscending(StyleRule rule1, StyleRule rule2) {
            if (rule1.specificity < rule2.specificity) return true;
            if (rule1.specificity == rule2.specificity) {
                if (rule1.fileNumber < rule2.fileNumber) return true;
                if (rule1.fileNumber == rule2.fileNumber) {
                    if (rule1.ruleNumber < rule2.ruleNumber) return true;
                }
            }
            return false;
        }

        StyleRule *StyleNode::findRule(int fileNumber, int ruleNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        return &(*listIt);
                    }
                }
            }
            return nullptr;
        }

        const StyleRule *StyleNode::findRule(int fileNumber, int ruleNumber) const {
            for (AppliedStyleMap::const_iterator it = style.cbegin(); it != style.cend(); it++) {
                for (StyleRules::const_iterator listIt = it->second.cbegin(); listIt != it->second.cend(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        return &(*listIt);
                    }
                }
            }
            return nullptr;
        }

        void StyleNode::addStyle(AppliedStyleMap &newStyle) {
            AppliedStyleMap::iterator actualStyleIt;
            // TODO: disable rule if affected by modifier
            for (AppliedStyleMap::iterator it = newStyle.begin(); it != newStyle.end(); it++) {
                actualStyleIt = style.find(it->first);
                if (actualStyleIt == style.cend()) { // not found in existing element's style or new style has bigger specificity
                    style[it->first] = it->second;
                }
                // same specificity, more recent file or same file and more recent rule
                else {
                    actualStyleIt->second.splice(actualStyleIt->second.end(), it->second);
                    actualStyleIt->second.sort(compareRulesPriorityDescending);
                }
            }
        }

        const AppliedStyleMap &StyleNode::getStyle() const { return style; }

        bool StyleNode::deleteStyle(int fileNumber, int ruleNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        it->second.erase(listIt);
                        return true; // should not have multiple rules with same file and rule number
                    }
                }
            }
            return false;
        }

        int StyleNode::deleteStyleFromFile(int fileNumber) {
            int nbDeletedRules = 0;
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end();) {
                    if (listIt->fileNumber == fileNumber) {
                        listIt = it->second.erase(listIt);
                        nbDeletedRules++;
                    }
                    else listIt++;
                }
            }
            return nbDeletedRules;
        }

        void StyleNode::updateStylePriorityFromFile(int oldFileNumber, int newFileNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == oldFileNumber) {
                        listIt->fileNumber = newFileNumber;
                    }
                }
                it->second.sort(compareRulesPriorityDescending);
            }
        }

        int StyleNode::clear() {
            int nbDeletedRules = 0;
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                nbDeletedRules += it->second.size();
                it->second.clear();
            }
            style.clear();
            return nbDeletedRules;
        }

        int StyleNode::nbRules() const {
            int nbRules = 0;
            for (AppliedStyleMap::const_iterator it = style.cbegin(); it != style.cend(); it++) {
                nbRules += it->second.size();
            }
            return nbRules;
        }

        bool StyleNode::getRule(const std::string &ruleName, StyleValue **ruleValue, bool canInherit, StyleValue *defaultStyle) const {
            AppliedStyleMap::const_iterator rules = style.find(ruleName);
            if (rules != nullptr) {
                for (StyleRules::const_iterator listIt = rules->second.cbegin(); listIt != rules->second.cend();
                     listIt++) { // find first enabled rule
                    if (listIt->enabled) {
                        *ruleValue = listIt->value;
                        return true;
                    }
                }
            }
            if (!canInherit || parent() == nullptr) {
                *ruleValue = defaultStyle;
                return (defaultStyle != nullptr);
            }
            return parent()->getRule(ruleName, ruleValue, canInherit, defaultStyle); // cascade style, if not found, check parent
        }

        bool StyleNode::getRule(const std::vector<std::string> &ruleNames, StyleValue **ruleValue, bool canInherit, StyleValue *defaultStyle) const {
            const StyleRule *currentRule = nullptr;
            for (const std::string &ruleName : ruleNames) {
                AppliedStyleMap::const_iterator rules = style.find(ruleName);
                if (rules != nullptr) {
                    for (StyleRules::const_iterator listIt = rules->second.cbegin(); listIt != rules->second.cend();
                         listIt++) { // find first enabled rule
                        if (listIt->enabled) {
                            if (currentRule == nullptr || compareRulesPriorityAscending(*currentRule, *listIt)) currentRule = &(*listIt);
                            break;
                        }
                    }
                }
            }
            if (currentRule != nullptr) {
                *ruleValue = currentRule->value;
                return true;
            }
            if (!canInherit || parent() == nullptr) {
                *ruleValue = defaultStyle;
                return (defaultStyle != nullptr);
            }
            return parent()->getRule(ruleNames, ruleValue, canInherit, defaultStyle); // cascade style, if not found, check parent
        }

        bool StyleNode::ruleExists(const std::string &ruleName) const { return style.find(ruleName) != nullptr; }

        bool StyleNode::ruleExists(int fileNumber, int ruleNumber) const { return findRule(fileNumber, ruleNumber); }

        void StyleNode::addSelector(std::string selectorName, StyleComponentType selectorType) {
            selectors.insert(StyleComponentData(selectorName, selectorType));
        }

        void StyleNode::addModifier(std::string modifierName) {
            modifiers.emplace(modifierName, std::pair<bool, std::list<std::pair<int, int>>>(false, {}));
        }

        void StyleNode::setModifierState(std::string modifierName, bool enabled) {
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::iterator modifier;
            modifier = modifiers.find(modifierName);
            if (modifier == modifiers.end() || modifier->second.first == enabled) return;
            modifier->second.first = enabled;
            for (std::pair<int, int> ruleIds : modifier->second.second) {
                toggleRule(ruleIds.first, ruleIds.second, enabled);
            }
        }

        void StyleNode::deactivateAllModifiers() { // seems useless
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::const_iterator modifiersIt;
            for (modifiersIt = modifiers.cbegin(); modifiersIt != modifiers.cend(); modifiersIt++) {
                setModifierState(modifiersIt->first, false);
            }
        }

        void StyleNode::addRuleAffectedByModifier(int fileNumber, int ruleNumber, std::string modifierName) {
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::iterator modifier;
            modifier = modifiers.find(modifierName);
            if (modifier == modifiers.end()) return;
            StyleRule *rule = findRule(fileNumber, ruleNumber);
            if (rule) rule->enabled = false;
            modifier->second.second.push_back(std::pair<int, int>(fileNumber, ruleNumber));
        }

        void StyleNode::toggleRule(int fileNumber, int ruleNumber) {
            StyleRule *rule = findRule(fileNumber, ruleNumber);
            if (rule) rule->enabled = !rule->enabled;
        }

        void StyleNode::toggleRule(int fileNumber, int ruleNumber, bool enabled) {
            StyleRule *rule = findRule(fileNumber, ruleNumber);
            if (rule) rule->enabled = enabled;
        }

        void StyleNode::setFontsPath(const std::string &path) {
            if (path[path.size() - 1] != '/') fontsPath = path + '/';
            else fontsPath = path;
        }

    } // namespace elementStyle
} // namespace style
