#ifndef PARSER_TOKENS_HPP
#define PARSER_TOKENS_HPP
#include <string>
#include <array>

namespace style::parser {
    enum class Token {
        NullRoot,
        StyleBlock,
        SelectorsBlock,
        SelectorsList,
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
        // value type
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

    std::string tokenToString(const Token &type);

    extern std::array<Token, 9> VALUE_TYPE_TOKENS;
} // style::parser

#endif // PARSER_TOKENS_HPP
