#include "abstract_configuration.hpp"

#include "parser.hpp"

namespace style::config {
    std::array<Token, 2> NESTABLE_TOKENS = {Token::Function, Token::Tuple};

    void configNodeChecker(const ConfigRuleNode *node) {
        if (!node) return;
        std::cerr << node << "\n";
        if (std::find(VALUE_TYPE_TOKENS.cbegin(), VALUE_TYPE_TOKENS.cend(), node->token()) == VALUE_TYPE_TOKENS.cend()) {
            throw InvalidStyleConfigException("'" + tokenToString(node->token()) + "' is not a valid token.");
        }
        std::cerr << "valid token\n";
        if (node->child()) {
            if (std::find(NESTABLE_TOKENS.cbegin(), NESTABLE_TOKENS.cend(), node->token()) == VALUE_TYPE_TOKENS.cend()) {
                throw InvalidStyleConfigException("Node with token '" + tokenToString(node->token()) + "' can't have childs");
            }
            std::cerr << "childs valids or no childs\n";
            std::cerr << "checking child " << node->child() << "\n";
            configNodeChecker(node->child());
            std::cerr << "child checked\n";
        }
        std::cerr << "checking next " << node->next() << "\n";
        configNodeChecker(node->next());
        std::cerr << "next checked\n";
    }

    void configChecker(const Config *config) {
        for (std::pair<std::string, const ConfigRuleNode *> rule : config->rules) {
            if (!Parser::isValidElementOrRuleName(rule.first)) throw InvalidStyleConfigException("'" + rule.first + "' is not a valid rule name");
            std::cerr << "checking node\n";
            configNodeChecker(rule.second);
            std::cerr << "node checked\n";
        }

        // TODO: units
    }
} // namespace style::config
