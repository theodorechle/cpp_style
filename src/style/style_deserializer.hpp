#ifndef STYLE_DESERIALIZER_HPP
#define STYLE_DESERIALIZER_HPP

#include "lexer.hpp"
#include "nodes_to_rules_blocks.hpp"
#include "parser.hpp"
#include "style_component.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace style {

    class StyleDeserializer {
    public:
        static std::list<StyleBlock *> *deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber);
        static std::list<StyleBlock *> *deserialize(const std::string &style, int fileNumber, int *ruleNumber);
    };

} // namespace Style

#endif // STYLE_DESERIALIZER_HPP
