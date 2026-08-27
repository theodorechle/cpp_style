#ifndef PARSER_HPP
#define PARSER_HPP

#include "deserialization_node.hpp"
#include <list>

namespace style::parser {
    enum class ParsingBlock { SELECTORS, FILE };

    std::string parsingBlockToString(ParsingBlock block);

    enum class ErrorType { WARNING, ERROR };

    std::string errorTypeToString(ErrorType type);

    struct ErrorMessage {
        ErrorType type;
        std::string message;
    };

    bool isValidElementOrRuleName(const std::string &str);

    struct ParseResult {
        DeserializationNode *node;
        std::list<ErrorMessage> *errors;
    };

    /*
        Tries to deserialize a DeserializationNode tree of the given block.
        Stop immediatly when this block is done.
        Returns nullptr if no matching block can be parsed.
    */
    ParseResult parse(DeserializationNode *currentNode, ParsingBlock block = ParsingBlock::FILE);

} // namespace style

#endif // PARSER_HPP
