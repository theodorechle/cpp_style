#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

namespace style::config {
    class InvalidStyleConfigException : public std::exception {
        std::string message;

    public:
        InvalidStyleConfigException(const std::string &message) : message{message} {};
        const char *what() const noexcept override { return message.c_str(); }
    };

    class ConfigRuleNode : public commons::Node<ConfigRuleNode> {
        Token _token;

    public:
        ConfigRuleNode(const Token token) : _token{token} {}
        const Token &token() const { return _token; }
    };

    class ConfigRuleNodeEnum : public ConfigRuleNode {
        const std::vector<std::string> _allowedValues;

    public:
        ConfigRuleNodeEnum(const std::vector<std::string> allowedValues) : ConfigRuleNode{Token::EnumValue}, _allowedValues{allowedValues} {}
        const std::vector<std::string> &allowedValues() const { return _allowedValues; }
    };

    struct Config {
        std::unordered_map<std::string, const ConfigRuleNode *> rules;
        std::vector<std::string> units;
    };

    extern std::array<Token, 2> NESTABLE_TOKENS;

    void configNodeChecker(const ConfigRuleNode *node);

    /*
    return whether the config is valid
    */
    void configChecker(const Config *config);
} // namespace style::config

#endif // ABSTRACT_CONFIGURATION_HPP