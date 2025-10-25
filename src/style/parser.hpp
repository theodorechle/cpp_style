#ifndef PARSER_HPP
#define PARSER_HPP

#include "node.hpp"
#include <algorithm>
#include <exception>
#include <string>
#include <unordered_map>

namespace style {

    class ParserException : public std::exception {
        std::string message;

    public:
        ParserException(const std::string &message) : message{message} {};
        const char *what() const noexcept override { return message.c_str(); }
    };

    class UnknownToken : public ParserException {
    public:
        UnknownToken(const Node &token) : ParserException{"Unknown token: \"" + token.getValue() + " (" + tokenToString(token.getToken()) + ")\""} {};
    };

    class MissingToken : public ParserException {
    public:
        MissingToken(const std::string &token) : ParserException{"Missing token: \"" + token + "\""} {};
    };

    class MalformedExpression : public ParserException {
    public:
        MalformedExpression(const std::string &expression) : ParserException{"Malformed expression: \"" + expression + "\""} {};
    };

    /**
     * Transform a chain of trees (no childs) like the one the lexer function returns into a tree containing the entire expression
     */
    class Parser {
        /**
         * The expressionTreeRoot should never contains a pointer pointing to currentNode in any way,
         * because it could be used and freed in the calling program after the parser call.
         * Consider currentNode as const
         */
        Node *currentNode;

        // only used to avoid recalculating many times the root
        Node *expressionTreeRoot = new Node(Token::NullRoot);
        Node *parsedTree = expressionTreeRoot;
        bool isValidName(const std::string &str, size_t start, size_t end);
        bool isValidElementOrRuleName(const std::string &str);
        bool isWhiteSpace(Token token);
        // relations are direct parent, any parent, same element, ...
        bool isComponentRelation(Token token);
        void removeSpace();
        void removeLineReturn();
        // removes all spaces and line returns childs
        void removeWhiteSpaces();

        void parse();

        void parseSpace();
        void parseLineBreak();
        void parseOneLineComment();
        void parseMultiLineComment();
        void parseValue();
        void parseComma();
        void parseColon();
        void parseSemiColon();
        void parseSharp();
        void parseDot();
        void parseAmpersand();
        void parseAt();
        void parseStar();
        void parseString();
        void parseGreatherThan();
        void parseOpeningParenthesis();
        void parseClosingParenthesis();
        void parseOpeningCurlyBracket();
        void parseClosingCurlyBracket();
        void parseRawName();
        void parseName();
        void parseUnit();

        // if you don't know how to use it, don't use it
        Node *updateLastDeclarationComponentBeforeNewOne(Node *lastChild);
        void parseDeclarationComponent(Token outputTokenType);
        void parseClass();
        void parseIdentifier();
        void parseModifier();

    public:
        Parser(Node *currentNode) : currentNode{currentNode} { parse(); };
        Node *getFinalTree() { return expressionTreeRoot; }
    };

} // namespace style

#endif // PARSER_HPP