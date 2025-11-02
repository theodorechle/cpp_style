#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "abstract_configuration.hpp"
#include "node.hpp"

constexpr int MAX_ERROR_COMPLEMENTARY_INFOS_SIZE = 20;

namespace style {

    class LexerException : public std::exception {
        std::string message;

    public:
        LexerException(const std::string &message) : message{message} {}
        const char *what() const noexcept override { return message.c_str(); };
    };

    class UnknownValue : public LexerException {
    public:
        UnknownValue(const std::string &value) : LexerException{"Error : Unknown value '" + value + "'"} {};
    };

    const std::map<char, Token> RESERVED_CHARACTERS = {
        {'(', Token::OpeningParenthesis},
        {')', Token::ClosingParenthesis},
        {'{', Token::OpeningCurlyBracket},
        {'}', Token::ClosingCurlyBracket},
        {',', Token::Comma},
        {':', Token::Colon},
        {';', Token::SemiColon},
        {'>', Token::GreaterThan},
        {'#', Token::Sharp},
        {'.', Token::Dot},
        {'&', Token::Ampersand},
        {'@', Token::At},
        {'*', Token::Star}
    };

    const std::vector<char> RAW_NAME_ALLOWED_SPECIAL_CHARACTERS = {'-', '_'};

    class Lexer {
        bool lexed;
        size_t index = 0;
        const Config *config;
        const std::string &expression;
        size_t expressionLength;
        Node *firstNode = new Node(Token::NullRoot);
        Node *parsedTree = firstNode;

    public:
        Lexer(const Config *config, const std::string &expression) : config{config}, expression{expression}, expressionLength{expression.length()} {
            lexe();
        };
        void lexe();
        void lexeSpace();
        void lexeLineReturn();
        void lexeOneLineComment();
        void lexeMultiLineComment();
        void lexeRawName();
        void lexeStringDoubleQuotes();
        void lexeStringSingleQuotes();
        void lexeInt();
        void lexeFloat();
        void lexeBool();
        std::string getUnit(int expressionIndex, int *size);
        void lexeUnit();
        void lexeReservedCharacters();
        Node *getResult() { return firstNode; }
    };

} // namespace style

#endif // TOKENIZER_HPP