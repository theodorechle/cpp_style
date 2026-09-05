#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer_node.hpp"
#include "parser_node.hpp"
#include <list>

namespace style::parser {
    enum class ParsingBlock { SELECTORS, FILE };

    std::string parsingBlockToString(ParsingBlock block);

    enum class ErrorType { WARNING, ERROR, LOG };

    std::string errorTypeToString(ErrorType type);

    struct ErrorMessage {
        ErrorType type;
        std::string message;

        // for line, 0 means unknown, since lines are 1-based
        size_t line = 0;
        size_t column = 0;
    };

    std::string errorMessageToString(ErrorMessage message);

    bool isValidElementOrRuleName(const std::string &str);

    struct ParseResult {
        ParserNode *node;
        std::list<ErrorMessage> *errors;
    };

    /*
        Tries to deserialize a ParserNode tree of the given block.
        Stop immediatly when this block is done.
        Returns nullptr if no matching block can be parsed.
    */
    ParseResult parse(lexer::LexerNode *currentNode, ParsingBlock block = ParsingBlock::FILE);

} // namespace style

#endif // PARSER_HPP
