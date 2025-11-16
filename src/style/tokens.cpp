#include <string>

#include "tokens.hpp"

namespace style {

    std::string tokenToString(const Token &type) {
        switch (type) {
        // Used only by the lexer
        case Token::OpeningParenthesis:
            return "OpeningParenthesis";
        case Token::ClosingParenthesis:
            return "ClosingParenthesis";
        case Token::OpeningCurlyBracket:
            return "OpeningCurlyBracket";
        case Token::ClosingCurlyBracket:
            return "ClosingCurlyBracket";
        case Token::RawName:
            return "RawName";
        case Token::Comma:
            return "Comma";
        case Token::Colon:
            return "Colon";
        case Token::SemiColon:
            return "SemiColon";
        case Token::GreaterThan:
            return "GreaterThan";
        case Token::OneLineComment:
            return "OneLineComment";
        case Token::MultiLineComment:
            return "MultiLineComment";
        case Token::Sharp:
            return "Sharp";
        case Token::Dot:
            return "Dot";
        case Token::Ampersand:
            return "Ampersand";
        case Token::At:
            return "At";
        case Token::Star:
            return "Star";
        // Used only by the parser
        case Token::StyleBlock:
            return "StyleBlock";
        case Token::BlockSelectors:
            return "BlockSelectors";
        case Token::Selector:
            return "Selector";
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
        // Used by both the lexer and the parser
        case Token::Space:
            return "Space";
        case Token::LineBreak:
            return "LineBreak";
        case Token::Empty:
            return "Empty";
        // NullRoot is used only when an algorithm must start with an existing node
        case Token::NullRoot:
            return "NullRoot";
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

    std::ostream &operator<<(std::ostream &o, const Token type) {
        o << tokenToString(type);
        return o;
    }

    std::array<Token, 9> VALUE_TYPE_TOKENS = {Token::EnumValue, Token::Tuple, Token::String, Token::Function, Token::Int,
                                    Token::Float,     Token::Bool,  Token::Unit,   Token::Hex};

} // namespace style
