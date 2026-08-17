#ifndef PARSER_HPP
#define PARSER_HPP

#include "deserialization_node.hpp"
#include <exception>
#include <string>

namespace style {

    class ParserException : public std::exception {
        std::string message;

    public:
        ParserException(const std::string &message) : message{message} {};
        const char *what() const noexcept override { return message.c_str(); }
    };

    class UnknownTokenException : public ParserException {
    public:
        UnknownTokenException(const DeserializationNode &token)
            : ParserException{"Unknown token: \"" + token.value() + " (" + tokenToString(token.token()) + ")\""} {};
    };

    class MissingTokenException : public ParserException {
    public:
        MissingTokenException(const std::string &token) : ParserException{"Missing token: \"" + token + "\""} {};
    };

    class MalformedExpressionException : public ParserException {
    public:
        MalformedExpressionException(const std::string &expression) : ParserException{"Malformed expression: \"" + expression + "\""} {};
    };

    enum class ParsingBlock { SELECTORS, FILE };

    /*
     * methods in the parser returning an instance of this structure should always set currentLexedNode.
     * If currentLexedNode is null, it should mean the parsing method has read all nodes.
     */
    struct ParsingState {
        const DeserializationNode *currentLexedNode = nullptr;
        DeserializationNode *currentParsedNode = nullptr;
    };

    /**
     * Transform a chain of trees (no childs) like the one the lexer function returns into a tree containing the entire expression
     */
    class Parser {
        static bool isValidName(const std::string &str, size_t start, size_t end);

    public:
        static bool isValidElementOrRuleName(const std::string &str);

        /*
            Tries to deserialize a DeserializationNode tree in the given block.
            Stop immediatly when this block is done.
            Returns nullptr if no matching block can be parsed.
        */
        DeserializationNode *parse(DeserializationNode *currentNode, ParsingBlock block = ParsingBlock::FILE);

    private:
        static ParsingState emptyParsingState(const DeserializationNode *currentLexedNode);
        // to check if spaces where removed
        static ParsingState removeSpaces(const DeserializationNode *currentLexedNode);

        static ParsingState tryParseElementName(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseClass(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseIdentifier(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseModifier(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseSelector(const DeserializationNode *currentLexedNode);

        static ParsingState tryParseDirectParentRelation(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseAnyParentRelation(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseSelectorsRelation(const DeserializationNode *currentLexedNode);

        static ParsingState tryParseSelectorsList(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseSelectorsBlock(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseRuleAssignment(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseRulesBlock(const DeserializationNode *currentLexedNode);

        static ParsingState tryParseImport(const DeserializationNode *currentLexedNode);
        static ParsingState tryParseAtRule(const DeserializationNode *currentLexedNode);

        static ParsingState tryParseFile(const DeserializationNode *currentLexedNode);
    };

} // namespace style

#endif // PARSER_HPP
