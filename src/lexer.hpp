#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <map>
#include <string>
#include <vector>

#include "abstract_configuration.hpp"
#include "lexer_node.hpp"

namespace style::lexer {
    constexpr int MAX_ERROR_COMPLEMENTARY_INFOS_SIZE = 20;
    constexpr std::string_view TRUE = "true";
    constexpr std::string_view FALSE = "false";

    class LexerException : public std::exception {
        std::string message;

    public:
        LexerException(const std::string &message) : message{message} {}
        const char *what() const noexcept override { return message.c_str(); };
    };

    class UnknownValue : public LexerException {
    public:
        UnknownValue(const std::string &value, size_t line, size_t column)
            : LexerException{"Error (at " + std::to_string(line) + ':' + std::to_string(column) + ": Unknown value '" + value + '\''} {};
    };

    const std::map<char, Token> RESERVED_CHARACTERS = {{'(', Token::OpeningParenthesis},
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
                                                       {'*', Token::Star}};

    const std::vector<char> RAW_NAME_ALLOWED_SPECIAL_CHARACTERS = {'-', '_'};

    class Lexer {
        const config::Config *_config = nullptr;
        size_t _index = 0;
        std::string _expression = "";
        lexer::LexerNode *_lexedTree = nullptr;
        size_t _line = 1;
        size_t _column = 0;

    public:
        lexer::LexerNode *lexe(const std::string &expression, const config::Config *config);
        size_t lexeSpace();
        size_t lexeLineReturn();
        size_t lexeOneLineComment();
        size_t lexeMultiLineComment();
        size_t lexeRawName();
        size_t lexeStringDoubleQuotes();
        size_t lexeStringSingleQuotes();
        size_t lexeInt();
        size_t lexeFloat();
        size_t lexeBool();
        std::string getUnit(int expressionIndex, int *size);
        size_t lexeUnit();
        size_t lexeReservedCharacters();
    };

} // namespace style::lexer

#endif // TOKENIZER_HPP
