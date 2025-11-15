#include "lexer.hpp"

namespace style {

    size_t Lexer::lexeSpace() {
        size_t i = 0;
        while (index + i < expression.length() && (expression[index + i] == ' ' || expression[index + i] == '\t')) {
            i++;
        }
        if (i > 0) {
            parsedTree->appendNext(new DeserializationNode(Token::Space));
        }
        return i;
    }

    size_t Lexer::lexeLineReturn() {
        size_t i = 0;
        while (index + i < expression.length() && expression[index + i] == '\n') {
            i++;
        }
        if (i == 0) return 0;
        parsedTree->appendNext(new DeserializationNode(Token::LineBreak));
        return i;
    }

    size_t Lexer::lexeOneLineComment() {
        if (expression[index] != '/' || expression[index + 1] != '/') return 0;
        size_t i = 1;
        while (index + i + 1 < expression.size() && expression[index + i + 1] != '\n') {
            i++;
        }
        parsedTree->appendNext(new DeserializationNode(Token::OneLineComment, expression.substr(index + 2, i - 1)));
        return i + 1;
    }

    size_t Lexer::lexeMultiLineComment() {
        if (expression[index] != '/' || index + 1 == expression.size() || expression[index + 1] != '*') return 0;
        size_t i = 1;
        while (index + i + 2 < expression.size() && !(expression[index + i + 1] == '*' && expression[index + i + 2] == '/')) {
            i++;
        }
        if (index + i + 2 >= expression.size()) return 0;
        parsedTree->appendNext(new DeserializationNode(Token::MultiLineComment, expression.substr(index + 2, i - 1)));
        return i + 3;
    }

    size_t Lexer::lexeRawName() {
        if (!std::isalnum(expression[index])) return 0;
        size_t i = 1;
        while (std::isalnum(expression[index + i])
               || std::find(RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cbegin(), RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend(), expression[index + i])
                      != RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend()) {
            i++;
        }
        parsedTree->appendNext(new DeserializationNode(Token::RawName, expression.substr(index, i)));
        return i;
    }

    size_t Lexer::lexeStringDoubleQuotes() {
        if (expression[index] != '"') return 0;
        size_t i = 1;
        while (index + i + 1 < expression.length() && expression[index + i + 1] != '"') {
            i++;
        }
        if (i != 1 && (index + i >= expression.length() || expression[index + i + 1] != '"')) return 0;
        parsedTree->appendNext(new DeserializationNode(Token::String, (i == 1) ? "" : expression.substr(index + 1, i)));
        return i + 2;
    }

    size_t Lexer::lexeStringSingleQuotes() {
        if (expression[index] != '\'') return 0;
        size_t i = 1;
        while (index + i + 1 < expression.length() && expression[index + i + 1] != '\'') {
            i++;
        }

        if (i != 1 && (index + i >= expression.length() || expression[index + i + 1] != '\'')) return 0;
        parsedTree->appendNext(new DeserializationNode(Token::String, (i == 1) ? "" : expression.substr(index + 1, i)));
        return i + 2;
    }

    size_t Lexer::lexeInt() {
        size_t i = 0;
        if (expression[index] == '-') i++;
        if (!isdigit(expression[index + i])) return 0;
        int tmpSize;
        while (index + i < expression.length() && isdigit(expression[index + i])) {
            i++;
        }
        if (index + i < expression.length()
            && RESERVED_CHARACTERS.find(expression[index + i]) == RESERVED_CHARACTERS.cend()
            && expression[index + i] != ' '
            && expression[index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return 0;
        parsedTree->appendNext(new DeserializationNode(Token::Int, expression.substr(index, i)));
        return i;
    }

    size_t Lexer::lexeFloat() {
        int tmpSize;
        bool dotFound = false;
        size_t i = 0;
        size_t min_index = 2;
        if (expression[index + i] == '-') {
            i++;
            min_index++;
        }
        while (index + i < expression.length()) {
            if (expression[index + i] == '.') {
                if (!dotFound) dotFound = true;
                else return 0;
            }
            else if (!isdigit(expression[index + i])) return 0;
            i++;
        }
        if (!dotFound || i < min_index) return 0; // need at least one int (0-9) and a dot
        if (index + i < expression.length()
            && RESERVED_CHARACTERS.find(expression[index + i]) == RESERVED_CHARACTERS.cend()
            && expression[index + i] != ' '
            && expression[index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return 0;
        parsedTree->appendNext(new DeserializationNode(Token::Float, expression.substr(index, i)));
        return i;
    }

    size_t Lexer::lexeBool() {
        if (expression.substr(index, TRUE.size()) == TRUE) {
            parsedTree->appendNext(new DeserializationNode(Token::Bool, expression.substr(index, TRUE.size())));
            return TRUE.size();
        }
        else if (expression.substr(index, FALSE.size()) == FALSE) {
            parsedTree->appendNext(new DeserializationNode(Token::Bool, expression.substr(index, FALSE.size())));
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
                if (expression[index + expressionIndex + i] != unit[i]) {
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
        parsedTree->appendNext(new DeserializationNode(Token::Unit, unit));
        return size;
    }

    size_t Lexer::lexeReservedCharacters() {
        std::map<char, Token>::const_iterator specialCharIt = RESERVED_CHARACTERS.find(expression[index]);
        if (specialCharIt == RESERVED_CHARACTERS.cend()) return 0;
        parsedTree->appendNext(new DeserializationNode(specialCharIt->second));
        return 1;
    }

    DeserializationNode *Lexer::lexe(const std::string &expression) {
        this->expression = expression;
        DeserializationNode *firstNode = new DeserializationNode(Token::NullRoot);
        parsedTree = firstNode;
        size_t increment;

        while (index < expression.length()) {
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
                throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
            }
            index += increment;
#ifdef DEBUG
            std::cerr << tokenToString(parsedTree->token()) << ": '" << parsedTree->value() << "'\n";
#endif
            parsedTree = parsedTree->next();
        }
        // remove the NullRoot token at the start
        DeserializationNode *nextList = firstNode->next();
        firstNode->next(nullptr);
        delete firstNode;
        return nextList;
    }

} // namespace style
