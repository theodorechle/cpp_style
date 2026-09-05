#include "parser_tokens.hpp"

namespace style::parser {
    std::string tokenToString(const Token &type) {
        switch (type) {
        case Token::StyleBlock:
            return "StyleBlock";
        case Token::SelectorsBlock:
            return "SelectorsBlock";
        case Token::SelectorsList:
            return "SelectorsList";
        case Token::BlockDeclarations:
            return "BlockDeclarations";
        case Token::DirectParent:
            return "DirectParent";
        case Token::AnyParent:
            return "AnyParent";
        case Token::SameElement:
            return "SameElement";
        case Token::RuleName:
            return "RuleName";
        case Token::Assignment:
            return "Assignment";
        case Token::ArgSeparator:
            return "ArgSeparator";
        case Token::StarWildcard:
            return "StarWildcard";
        case Token::ElementName:
            return "ElementName";
        case Token::Name:
            return "Name";
        case Token::Class:
            return "Class";
        case Token::Modifier:
            return "Modifier";
        case Token::Identifier:
            return "Identifier";
        case Token::Import:
            return "Import";
        // value types
        case Token::Tuple:
            return "Tuple";
        case Token::String:
            return "String";
        case Token::Function:
            return "Function";
        case Token::Int:
            return "Int";
        case Token::Float:
            return "Float";
        case Token::Bool:
            return "Bool";
        case Token::Unit:
            return "Unit";
        case Token::Hex:
            return "Hex";
        case Token::EnumValue:
            return "EnumValue";
        default:
            return "Unknown";
        }
    }

    std::array<Token, 9> VALUE_TYPE_TOKENS = {Token::EnumValue, Token::Tuple, Token::String, Token::Function, Token::Int,
                                              Token::Float,     Token::Bool,  Token::Unit,   Token::Hex};
} // style::parser
