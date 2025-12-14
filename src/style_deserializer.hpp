#ifndef STYLE_DESERIALIZER_HPP
#define STYLE_DESERIALIZER_HPP

#include "lexer.hpp"
#include "nodes_to_style_components.hpp"
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
        static std::list<StyleDefinition *> *deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber,
                                                            const config::Config *config);
        static std::list<StyleDefinition *> *deserialize(const std::string &style, int fileNumber, int *ruleNumber, const config::Config *config);
    };

} // namespace Style

#endif // STYLE_DESERIALIZER_HPP
