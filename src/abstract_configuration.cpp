#include "abstract_configuration.hpp"

#include "parser.hpp"
#include "parser_tokens.hpp"
#include <algorithm>

namespace style::config {
    std::array<parser::Token, 2> NESTABLE_TOKENS = {parser::Token::Function, parser::Token::Tuple};

    void configNodeChecker(const ConfigRuleNode *node) {
        if (!node) return;
        if (std::find(parser::VALUE_TYPE_TOKENS.cbegin(), parser::VALUE_TYPE_TOKENS.cend(), node->token()) == parser::VALUE_TYPE_TOKENS.cend()) {
            throw InvalidStyleConfigException("'" + tokenToString(node->token()) + "' is not a valid value token.");
        }
        if (node->child()) {
            if (std::find(NESTABLE_TOKENS.cbegin(), NESTABLE_TOKENS.cend(), node->token()) == parser::VALUE_TYPE_TOKENS.cend()) {
                throw InvalidStyleConfigException("Node with token '" + tokenToString(node->token()) + "' can't have children");
            }
            configNodeChecker(node->child());
        }
        configNodeChecker(node->next());
    }

    void configChecker(const Config *config) {
        for (std::pair<std::string, std::vector<const ConfigRuleNode *>> rule : config->rules) {
            if (!parser::isValidElementOrRuleName(rule.first)) throw InvalidStyleConfigException("'" + rule.first + "' is not a valid rule name");
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

    Config::~Config() {
        for (std::pair<std::string, std::vector<const ConfigRuleNode *>> rule : rules) {
            for (const ConfigRuleNode *value : rule.second) {
                delete value;
            }
        }
    }
} // namespace style::config
