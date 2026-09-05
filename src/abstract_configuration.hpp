#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include "../cpp_commons/src/node.hpp"
#include "parser_tokens.hpp"
#include <exception>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace style::config {
    class InvalidStyleConfigException : public std::exception {
        std::string message;

    public:
        InvalidStyleConfigException(const std::string &message) : message{message} {};
        const char *what() const noexcept override { return message.c_str(); }
    };

    class ConfigRuleNode : public commons::Node<ConfigRuleNode> {
        parser::Token _token;

    protected:
        std::string debugValue() const override { return parser::tokenToString(_token); }

    public:
        ConfigRuleNode(const parser::Token token, ConfigRuleNode *child = nullptr, ConfigRuleNode *next = nullptr)
            : commons::Node<ConfigRuleNode>(child, next), _token{token} {}
        const parser::Token &token() const { return _token; }
    };

    class ConfigRuleNodeEnum : public ConfigRuleNode {
        const std::set<std::string> _allowedValues;

        std::string debugValue() const override;

    public:
        ConfigRuleNodeEnum(const std::set<std::string> allowedValues) : ConfigRuleNode{parser::Token::EnumValue}, _allowedValues{allowedValues} {}
        const std::set<std::string> &allowedValues() const { return _allowedValues; }
    };

    struct Config {
        std::unordered_map<std::string, std::vector<const ConfigRuleNode *>> rules;
        std::vector<std::string> units;
        std::unordered_set<std::string> inheritableRules;

        ~Config();
    };

    extern std::array<parser::Token, 2> NESTABLE_TOKENS;

    void configNodeChecker(const ConfigRuleNode *node);

    /*
    return whether the config is valid
    */
    void configChecker(const Config *config);
} // namespace style::config

#endif // ABSTRACT_CONFIGURATION_HPP
