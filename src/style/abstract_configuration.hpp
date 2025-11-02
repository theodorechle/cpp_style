#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace style
{
    struct Rule {
        // replace with list of trees of allowed tokens
        bool string;
        bool intNumber;
        bool floatNumber;
        bool withUnit;
        bool hexadecimal; // + tuples
        std::vector<std::string> enumValues;
    };

    struct Config {
        std::unordered_map<std::string, Rule> rules;
        std::vector<std::string> units;
    };
} // namespace style

#endif // ABSTRACT_CONFIGURATION_HPP