#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <array>
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
        BlockSelectors,
        Selector,
        BlockDeclarations,
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
        Unit,
        Hex,
    };

    extern std::array<Token, 9> VALUE_TYPE_TOKENS;

    constexpr std::string_view TRUE = "true";
    constexpr std::string_view FALSE = "false";

    std::string tokenToString(const Token &type);

    std::ostream &operator<<(std::ostream &o, const Token type);

} // namespace style

#endif // TOKENS_HPP