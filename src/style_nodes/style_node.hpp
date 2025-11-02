#ifndef STYLE_NODE_HPP
#define STYLE_NODE_HPP

#include "../../cpp_commons/src/node.hpp"
#include "../style/style_component.hpp"
#include <algorithm>
#include <set>
#include <unordered_map>

namespace style {
    namespace elementStyle {

        typedef std::list<StyleRule> StyleRules;
        typedef std::unordered_map<std::string, StyleRules> AppliedStyleMap;

        class StyleNode : public commons::Node<StyleNode> {
            AppliedStyleMap style = {};
            /**
             * key:
             *  modifier name
             * value:
             *  isActive
             *  list: // all rules who are affected
             *   first: fileNumber
             *   second: ruleNumber
             */
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>> modifiers = {};
            std::set<StyleComponentData> selectors = {};

            std::string fontsPath = "/";

            static bool compareRulesPriorityDescending(StyleRule rule1, StyleRule rule2); // TODO: take as references
            static bool compareRulesPriorityAscending(StyleRule rule1, StyleRule rule2);
            StyleRule *findRule(int fileNumber, int ruleNumber);
            const StyleRule *findRule(int fileNumber, int ruleNumber) const;

        public:
            void setStyle(const AppliedStyleMap &style) { this->style = style; }
            void addStyle(AppliedStyleMap &newStyle);
            const AppliedStyleMap &getStyle() const;
            /*
            Delete first rule who correspond to the file and rule number.
            Each rule should have a unique number.
            If not, which one is deleted cannot be guaranted.

            returns true if successfully deleted, false else
            */
            bool deleteStyle(int fileNumber, int ruleNumber);
            // returns the number of deleted rules
            int deleteStyleFromFile(int fileNumber);
            void updateStylePriorityFromFile(int oldFileNumber, int newFileNumber);
            int clear();
            int nbRules() const;
            /**
             * Set the value in the ruleValue parameter.
             * Returns true if found or default value is returned.
             * If canInherit is true and rule is not found, tries to search in parents style.
             * If no defaultStyle is given, returns false if no value were found.
             */
            bool getRule(const std::string &ruleName, StyleValue **ruleValue, bool canInherit = false, StyleValue *defaultStyle = nullptr) const;
            /**
             * Set the value in the ruleValue parameter.
             * Returns true if found or default value is returned.
             * If canInherit is true and rule is not found, tries to search in parents style.
             * If no defaultStyle is given, returns false if no value were found.
             *
             * Return the value with biggest specificity among all matching rules.
             * If two values have same specificity, first in the rulesNames list will be prioritized.
             */
            bool getRule(const std::vector<std::string> &ruleNames, StyleValue **ruleValue, bool canInherit = false,
                         StyleValue *defaultStyle = nullptr) const;
            bool ruleExists(const std::string &ruleName) const;
            bool ruleExists(int fileNumber, int ruleNumber) const;
            const std::set<StyleComponentData> *getSelectors() { return &selectors; }
            void addSelector(std::string selectorName, StyleComponentType selectorType);
            void addModifier(std::string modifierName);
            void setModifierState(std::string modifierName, bool enabled);
            void deactivateAllModifiers();
            void addRuleAffectedByModifier(int fileNumber, int ruleNumber, std::string modifierName);
            void toggleRule(int fileNumber, int ruleNumber);
            void toggleRule(int fileNumber, int ruleNumber, bool enabled);

            void setFontsPath(const std::string &path);
            const std::string &getFontsPath() const { return fontsPath; }
        };

    } // namespace elementStyle
} // namespace style

#endif // STYLE_NODE_HPP
