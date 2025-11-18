#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <exception>
#include <set>
#include <string>
#include <unordered_map>

namespace style::config {
    class InvalidStyleConfigException : public std::exception {
        std::string message;

    public:
        InvalidStyleConfigException(const std::string &message) : message{message} {};
        const char *what() const noexcept override { return message.c_str(); }
    };

    class ConfigRuleNode : public commons::Node<ConfigRuleNode> {
        Token _token;

    protected:
        std::string debugValue() const override { return style::tokenToString(_token); }

    public:
        ConfigRuleNode(const Token token) : _token{token} {}
        const Token &token() const { return _token; }
    };

    class ConfigRuleNodeEnum : public ConfigRuleNode {
        const std::set<std::string> _allowedValues;

        std::string debugValue() const override;

    public:
        ConfigRuleNodeEnum(const std::set<std::string> allowedValues) : ConfigRuleNode{Token::EnumValue}, _allowedValues{allowedValues} {}
        const std::set<std::string> &allowedValues() const { return _allowedValues; }
    };

    struct Config {
        std::unordered_map<std::string, std::vector<const ConfigRuleNode *>> rules;
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