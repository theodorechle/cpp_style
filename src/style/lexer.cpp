#include "lexer.hpp"

namespace style {

    size_t Lexer::lexeSpace() {
        size_t i = 0;
        while (_index + i < _expression.length() && (_expression[_index + i] == ' ' || _expression[_index + i] == '\t')) {
            i++;
        }
        if (i > 0) {
            _parsedTree->appendNext(new DeserializationNode(Token::Space));
        }
        return i;
    }

    size_t Lexer::lexeLineReturn() {
        size_t i = 0;
        while (_index + i < _expression.length() && _expression[_index + i] == '\n') {
            i++;
        }
        if (i == 0) return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::LineBreak));
        return i;
    }

    size_t Lexer::lexeOneLineComment() {
        if (_expression[_index] != '/' || _expression[_index + 1] != '/') return 0;
        size_t i = 1;
        while (_index + i + 1 < _expression.size() && _expression[_index + i + 1] != '\n') {
            i++;
        }
        _parsedTree->appendNext(new DeserializationNode(Token::OneLineComment, _expression.substr(_index + 2, i - 1)));
        return i + 1;
    }

    size_t Lexer::lexeMultiLineComment() {
        if (_expression[_index] != '/' || _index + 1 == _expression.size() || _expression[_index + 1] != '*') return 0;
        size_t i = 1;
        while (_index + i + 2 < _expression.size() && !(_expression[_index + i + 1] == '*' && _expression[_index + i + 2] == '/')) {
            i++;
        }
        if (_index + i + 2 >= _expression.size()) return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::MultiLineComment, _expression.substr(_index + 2, i - 1)));
        return i + 3;
    }

    size_t Lexer::lexeRawName() {
        if (!std::isalnum(_expression[_index])) return 0;
        size_t i = 1;
        while (std::isalnum(_expression[_index + i])
               || std::find(RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cbegin(), RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend(), _expression[_index + i])
                      != RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend()) {
            i++;
        }
        _parsedTree->appendNext(new DeserializationNode(Token::RawName, _expression.substr(_index, i)));
        return i;
    }

    size_t Lexer::lexeStringDoubleQuotes() {
        if (_expression[_index] != '"') return 0;
        size_t i = 1;
        while (_index + i + 1 < _expression.length() && _expression[_index + i + 1] != '"') {
            i++;
        }
        if (i != 1 && (_index + i >= _expression.length() || _expression[_index + i + 1] != '"')) return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::String, (i == 1) ? "" : _expression.substr(_index + 1, i)));
        return i + 2;
    }

    size_t Lexer::lexeStringSingleQuotes() {
        if (_expression[_index] != '\'') return 0;
        size_t i = 1;
        while (_index + i + 1 < _expression.length() && _expression[_index + i + 1] != '\'') {
            i++;
        }

        if (i != 1 && (_index + i >= _expression.length() || _expression[_index + i + 1] != '\'')) return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::String, (i == 1) ? "" : _expression.substr(_index + 1, i)));
        return i + 2;
    }

    size_t Lexer::lexeInt() {
        size_t i = 0;
        if (_expression[_index] == '-') i++;
        if (!isdigit(_expression[_index + i])) return 0;
        int tmpSize;
        while (_index + i < _expression.length() && isdigit(_expression[_index + i])) {
            i++;
        }
        if (_index + i < _expression.length()
            && RESERVED_CHARACTERS.find(_expression[_index + i]) == RESERVED_CHARACTERS.cend()
            && _expression[_index + i] != ' '
            && _expression[_index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::Int, _expression.substr(_index, i)));
        return i;
    }

    size_t Lexer::lexeFloat() {
        int tmpSize;
        bool dotFound = false;
        size_t i = 0;
        size_t min_index = 2;
        if (_expression[_index + i] == '-') {
            i++;
            min_index++;
        }
        while (_index + i < _expression.length()) {
            if (_expression[_index + i] == '.') {
                if (!dotFound) dotFound = true;
                else return 0;
            }
            else if (!isdigit(_expression[_index + i])) return 0;
            i++;
        }
        if (!dotFound || i < min_index) return 0; // need at least one int (0-9) and a dot
        if (_index + i < _expression.length()
            && RESERVED_CHARACTERS.find(_expression[_index + i]) == RESERVED_CHARACTERS.cend()
            && _expression[_index + i] != ' '
            && _expression[_index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::Float, _expression.substr(_index, i)));
        return i;
    }

    size_t Lexer::lexeBool() {
        if (_expression.substr(_index, TRUE.size()) == TRUE) {
            _parsedTree->appendNext(new DeserializationNode(Token::Bool, _expression.substr(_index, TRUE.size())));
            return TRUE.size();
        }
        else if (_expression.substr(_index, FALSE.size()) == FALSE) {
            _parsedTree->appendNext(new DeserializationNode(Token::Bool, _expression.substr(_index, FALSE.size())));
            return FALSE.size();
        }
        return 0;
    }

    std::string Lexer::getUnit(int expressionIndex, int *size) {
        size_t i;
        bool equal;
        // TODO: either use config or parse all possible units (need a definition of what a unit can be)
        constexpr std::string_view PIXEL_UNIT = "px";
        constexpr std::string_view PERCENTAGE_UNIT = "%";
        for (const std::string &unit : {std::string(PIXEL_UNIT), std::string(PERCENTAGE_UNIT)}) {
            equal = true;
            for (i = 0; i < unit.size(); i++) {
                if (_expression[_index + expressionIndex + i] != unit[i]) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                *size = i;
                return unit;
            }
        }
        *size = 0;
        return "";
    }

    size_t Lexer::lexeUnit() {
        int size;
        std::string unit = getUnit(0, &size);
        if (!unit.size()) return 0;
        _parsedTree->appendNext(new DeserializationNode(Token::Unit, unit));
        return size;
    }

    size_t Lexer::lexeReservedCharacters() {
        std::map<char, Token>::const_iterator specialCharIt = RESERVED_CHARACTERS.find(_expression[_index]);
        if (specialCharIt == RESERVED_CHARACTERS.cend()) return 0;
        _parsedTree->appendNext(new DeserializationNode(specialCharIt->second));
        return 1;
    }

    DeserializationNode *Lexer::lexe(const std::string &expression) {
        _expression = expression;
        DeserializationNode *firstNode = new DeserializationNode(Token::NullRoot);
        _parsedTree = firstNode;
        size_t increment;

        while (_index < expression.length()) {
            increment = lexeSpace();
            if (!increment) increment = lexeLineReturn();
            if (!increment) increment = lexeOneLineComment();
            if (!increment) increment = lexeMultiLineComment();
            if (!increment) increment = lexeStringDoubleQuotes();
            if (!increment) increment = lexeStringSingleQuotes();
            if (!increment) increment = lexeFloat();
            if (!increment) increment = lexeInt();
            if (!increment) increment = lexeBool();
            if (!increment) increment = lexeUnit();
            if (!increment) increment = lexeRawName();
            if (!increment) increment = lexeReservedCharacters();
            if (!increment) {
                delete firstNode;
                firstNode = nullptr;
                throw UnknownValue(expression.substr(_index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
            }
            _index += increment;
#ifdef DEBUG
            std::cerr << tokenToString(_parsedTree->token()) << ": '" << _parsedTree->value() << "'\n";
#endif
            _parsedTree = _parsedTree->next();
        }
        // remove the NullRoot token at the start
        DeserializationNode *nextList = firstNode->next();
        firstNode->next(nullptr);
        delete firstNode;
        return nextList;
    }

} // namespace style
