#ifndef LEXER_TOKENS_HPP
#define LEXER_TOKENS_HPP

#include <string>

namespace style::lexer {
    enum class Token {
        NullRoot,
        OpeningParenthesis,
        ClosingParenthesis,
        OpeningCurlyBracket,
        ClosingCurlyBracket,
        RawName,
        Comma,
        Colon,
        SemiColon,
        GreaterThan,
        OneLineComment,
        MultiLineComment,
        Sharp,
        Dot,
        Ampersand,
        At,
        Star,
        Space,
        LineBreak,
        String,
        Int,
        Float,
        Bool,
        Unit
    };

    std::string tokenToString(const Token &type);
} // style::lexer

#endif // LEXER_TOKENS_HPP
