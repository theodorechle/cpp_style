#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace style::config {
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
        std::unordered_map<std::string, ConfigRuleNode> rules;
        std::vector<std::string> units;
    };

    /*
    return whether the config is valid
    */
    bool configChecker(Config config);
} // namespace style::config

#endif // ABSTRACT_CONFIGURATION_HPP