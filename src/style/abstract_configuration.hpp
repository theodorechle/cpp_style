#ifndef ABSTRACT_CONFIGURATION_HPP
#define ABSTRACT_CONFIGURATION_HPP

#include "../../cpp_commons/src/value_node.hpp"
#include "tokens.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace style {
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
        std::unordered_map<std::string, commons::ValueNode<Token>> rules;
        std::vector<std::string> units;
    };
} // namespace style

#endif // ABSTRACT_CONFIGURATION_HPP