#include "lexer_tokens.hpp"

namespace style::lexer {
    std::string tokenToString(const Token &type) {
        switch (type) {
        // NullRoot is used only when an algorithm must start with an existing node
        case Token::NullRoot:
            return "NullRoot";
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
        case Token::Space:
            return "Space";
        case Token::LineBreak:
            return "LineBreak";
        case Token::String:
            return "String";
        case Token::Int:
            return "Int";
        case Token::Float:
            return "Float";
        case Token::Bool:
            return "Bool";
        case Token::Unit:
            return "Unit";
        default:
            return "Unknown";
        }
    }
} // style::lexer
