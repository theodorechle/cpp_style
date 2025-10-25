#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <iostream>
#include <map>
#include <string>

namespace style {

    enum class Token {
        // Used only by the lexer
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
        // Used only by the parser
        StyleBlock,
        BlockDeclaration,
        Declaration,
        BlockDefinition,
        DirectParent,
        AnyParent,
        SameElement,
        RuleName,
        Assignment,
        ArgSeparator,
        StarWildcard,
        ElementName,
        Name,
        Class,
        Modifier,
        Identifier,
        Import,
        // Used by both the lexer and the parser
        Space,
        LineBreak,
        Empty,
        // NullRoot is used only when an algorithm must start with an existing node
        NullRoot,
        // value types
        EnumValue,
        Tuple,
        String,
        Function,
        Int,
        Float,
        Bool,
        PercentageUnit,
        PixelUnit,
        Hex,
    };

    const std::map<std::string, Token> UNITS{{"%", Token::PercentageUnit}, {"px", Token::PixelUnit}};

    const std::string TRUE = "true";
    const std::string FALSE = "false";

    std::string tokenToString(const Token &type);

    std::ostream &operator<<(std::ostream &o, const Token type);

} // namespace Style

#endif // TOKENS_HPP