#include "lexer.hpp"

namespace style {

    void Lexer::lexeSpace() {
        size_t i = 0;
        while (index + i < expressionLength && (expression[index + i] == ' ' || expression[index + i] == '\t')) {
            i++;
        }
        if (i > 0) {
            parsedTree->appendNext(new Node(Token::Space));
            lexed = true;
            index += i;
        }
    }

    void Lexer::lexeLineReturn() {
        size_t i = 0;
        while (index + i < expressionLength && expression[index + i] == '\n') {
            i++;
        }
        if (i == 0) return;
        parsedTree->appendNext(new Node(Token::LineBreak));
        lexed = true;
        index += i;
    }

    void Lexer::lexeOneLineComment() {
        if (expression[index] != '/' || expression[index + 1] != '/') return;
        int i = 1;
        while (index + i + 1 < expression.size() && expression[index + i + 1] != '\n') {
            i++;
        }
        parsedTree->appendNext(new Node(Token::OneLineComment, expression.substr(index + 2, i - 1)));
        lexed = true;
        index += i + 1;
    }

    void Lexer::lexeMultiLineComment() {
        if (expression[index] != '/' || index + 1 == expression.size() || expression[index + 1] != '*') return;
        int i = 1;
        while (index + i + 2 < expression.size() && !(expression[index + i + 1] == '*' && expression[index + i + 2] == '/')) {
            i++;
        }
        if (index + i + 2 >= expression.size()) return;
        parsedTree->appendNext(new Node(Token::MultiLineComment, expression.substr(index + 2, i - 1)));
        lexed = true;
        index += i + 3;
    }

    void Lexer::lexeRawName() {
        if (!std::isalnum(expression[index])) return;
        size_t i = 1;
        while (std::isalnum(expression[index + i])
               || std::find(RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cbegin(), RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend(), expression[index + i])
                      != RAW_NAME_ALLOWED_SPECIAL_CHARACTERS.cend()) {
            i++;
        }
        parsedTree->appendNext(new Node(Token::RawName, expression.substr(index, i)));
        index += i;
        lexed = true;
    }

    void Lexer::lexeStringDoubleQuotes() {
        if (expression[index] != '"') return;
        int i = 1;
        while (index + i + 1 < expressionLength && expression[index + i + 1] != '"') {
            i++;
        }
        if (i != 1 && (index + i >= expressionLength || expression[index + i + 1] != '"')) return;
        parsedTree->appendNext(new Node(Token::String, (i == 1) ? "" : expression.substr(index + 1, i)));
        lexed = true;
        index += i + 2;
    }

    void Lexer::lexeStringSingleQuotes() {
        if (expression[index] != '\'') return;
        int i = 1;
        while (index + i + 1 < expressionLength && expression[index + i + 1] != '\'') {
            i++;
        }

        if (i != 1 && (index + i >= expressionLength || expression[index + i + 1] != '\'')) return;
        parsedTree->appendNext(new Node(Token::String, (i == 1) ? "" : expression.substr(index + 1, i)));
        lexed = true;
        index += i + 2;
    }

    void Lexer::lexeInt() {
        size_t i = 0;
        if (expression[index] == '-') i++;
        if (!isdigit(expression[index + i])) return;
        int tmpSize;
        while (index + i < expressionLength && isdigit(expression[index + i])) {
            i++;
        }
        if (index + i < expressionLength
            && RESERVED_CHARACTERS.find(expression[index + i]) == RESERVED_CHARACTERS.cend()
            && expression[index + i] != ' '
            && expression[index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return; // not an int
        parsedTree->appendNext(new Node(Token::Int, expression.substr(index, i)));
        index += i;
        lexed = true;
    }

    void Lexer::lexeFloat() {
        int tmpSize;
        bool dotFound = false;
        size_t i = 0;
        size_t min_index = 2;
        if (expression[index + i] == '-') {
            i++;
            min_index++;
        }
        while (index + i < expressionLength) {
            if (expression[index + i] == '.') {
                if (!dotFound) dotFound = true;
                else return;
            }
            else if (!isdigit(expression[index + i])) return;
            i++;
        }
        if (!dotFound || i < min_index) return; // need at least one int (0-9) and a dot
        if (index + i < expressionLength
            && RESERVED_CHARACTERS.find(expression[index + i]) == RESERVED_CHARACTERS.cend()
            && expression[index + i] != ' '
            && expression[index + i] != '\n'
            && !getUnit(i, &tmpSize).size())
            return; // not a float
        parsedTree->appendNext(new Node(Token::Float, expression.substr(index, i)));
        index += i;
        lexed = true;
    }

    void Lexer::lexeBool() {
        if (expression.substr(index, TRUE.size()) == TRUE) {
            parsedTree->appendNext(new Node(Token::Bool, expression.substr(index, TRUE.size())));
            index += TRUE.size();
            lexed = true;
        }
        else if (expression.substr(index, FALSE.size()) == FALSE) {
            parsedTree->appendNext(new Node(Token::Bool, expression.substr(index, FALSE.size())));
            index += FALSE.size();
            lexed = true;
        }
    }

    std::string Lexer::getUnit(int expressionIndex, int *size) {
        size_t i;
        bool equal;
        for (const std::string &unit : config->units) {
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

    void Lexer::lexeUnit() {
        int size;
        std::string unit = getUnit(0, &size);
        if (!unit.size()) return;
        index += size;
        lexed = true;
        parsedTree->appendNext(new Node(Token::Unit, unit));
    }

    void Lexer::lexeReservedCharacters() {
        std::map<char, Token>::const_iterator specialCharIt = RESERVED_CHARACTERS.find(expression[index]);
        if (specialCharIt == RESERVED_CHARACTERS.cend()) return;
        parsedTree->appendNext(new Node(specialCharIt->second));
        index++;
        lexed = true;
    }

    void Lexer::lexe() {
        while (index < expressionLength) {
            lexed = false;
            lexeSpace();
            if (!lexed) lexeLineReturn();
            if (!lexed) lexeOneLineComment();
            if (!lexed) lexeMultiLineComment();
            if (!lexed) lexeStringDoubleQuotes();
            if (!lexed) lexeStringSingleQuotes();
            if (!lexed) lexeFloat();
            if (!lexed) lexeInt();
            if (!lexed) lexeBool();
            if (!lexed) lexeUnit();
            if (!lexed) lexeRawName();
            if (!lexed) lexeReservedCharacters();
            if (!lexed) {
                delete firstNode;
                firstNode = nullptr;
                throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
            }
#ifdef DEBUG
            std::cerr << tokenToString(parsedTree->getToken()) << ": '" << parsedTree->getValue() << "'\n";
#endif
            parsedTree = parsedTree->next();
        }
        // remove the NullRoot token at the start
        Node *nextList = firstNode->next();
        firstNode->next(nullptr);
        delete firstNode;
        firstNode = nextList;
    }

} // namespace style
