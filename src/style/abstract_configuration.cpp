#include "abstract_configuration.hpp"

#include "parser.hpp"

namespace style::config {
    std::array<Token, 2> NESTABLE_TOKENS = {Token::Function, Token::Tuple};

    void configNodeChecker(const ConfigRuleNode *node) {
        if (!node) return;
        if (std::find(VALUE_TYPE_TOKENS.cbegin(), VALUE_TYPE_TOKENS.cend(), node->token()) == VALUE_TYPE_TOKENS.cend()) {
            throw InvalidStyleConfigException("'" + tokenToString(node->token()) + "' is not a valid token.");
        }
        if (node->child()) {
            if (std::find(NESTABLE_TOKENS.cbegin(), NESTABLE_TOKENS.cend(), node->token()) == VALUE_TYPE_TOKENS.cend()) {
                throw InvalidStyleConfigException("Node with token '" + tokenToString(node->token()) + "' can't have childs");
            }
            configNodeChecker(node->child());
        }
        configNodeChecker(node->next());
    }

    void configChecker(const Config *config) {
        for (std::pair<std::string, std::vector<const ConfigRuleNode *>> rule : config->rules) {
            if (!Parser::isValidElementOrRuleName(rule.first)) throw InvalidStyleConfigException("'" + rule.first + "' is not a valid rule name");
            for (const ConfigRuleNode *configRule : rule.second) {
                configNodeChecker(configRule);
            }
        }

        // TODO: units
    }
    std::string ConfigRuleNodeEnum::debugValue() const {
        std::string value = ConfigRuleNode::debugValue() + " (";
        for (std::string allowedValue : _allowedValues) {
            value += allowedValue + ",";
        }
        value += ")";
        return value;
    }
} // namespace style::config
