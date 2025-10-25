#include "parser.hpp"

namespace style {

    bool Parser::isValidName(const std::string &str, size_t start, size_t end) {
        size_t i;
        end = std::min(end, str.size());
        for (i = start; i < end - 1; i++) {
            if (!isalpha(str[i]) && str[i] != '_' && str[i] != '-') return false;
        }
        if (!isalpha(str[i]) && str[i] != '_') return false;
        return true;
    }

    bool Parser::isValidElementOrRuleName(const std::string &str) {
        if (!isalpha(str[0])) return false;
        return str.size() == 1 || isValidName(str, 1, str.size());
    }

    bool Parser::isWhiteSpace(Token token) { return (token == Token::Space || token == Token::LineBreak); }

    bool Parser::isComponentRelation(Token token) { return (token == Token::AnyParent || token == Token::DirectParent); }

    void Parser::removeSpace() {
        Node *lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::Space) parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeLineReturn() {
        Node *lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::LineBreak) parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeWhiteSpaces() {
        Node *lastChild = parsedTree->getLastChild();
        while (lastChild != nullptr && (lastChild->getToken() == Token::Space || lastChild->getToken() == Token::LineBreak)) {
            parsedTree->deleteSpecificChild(lastChild);
            lastChild = parsedTree->getLastChild();
        }
    }

    void Parser::parse() {
        try {
            while (currentNode != nullptr) {
#ifdef DEBUG
                std::cerr << "\nActual token : " << tokenToString(currentNode->getToken()) << ": '" << currentNode->getValue() << "'" << "\n";
#endif
                switch (currentNode->getToken()) {
                case Token::Space:
                    parseSpace();
                    break;
                case Token::LineBreak:
                    parseLineBreak();
                    break;
                case Token::OneLineComment:
                    parseOneLineComment();
                    break;
                case Token::MultiLineComment:
                    parseMultiLineComment();
                    break;
                case Token::String:
                    parseString();
                    break;
                case Token::Int:
                case Token::Float:
                case Token::Bool:
                    parseValue();
                    break;
                case Token::RawName:
                    parseRawName();
                    break;
                case Token::Comma:
                    parseComma();
                    break;
                case Token::Colon:
                    parseColon();
                    break;
                case Token::SemiColon:
                    parseSemiColon();
                    break;
                case Token::Sharp:
                    parseSharp();
                    break;
                case Token::Dot:
                    parseDot();
                    break;
                case Token::Ampersand:
                    parseAmpersand();
                    break;
                case Token::At:
                    parseAt();
                    break;
                case Token::Star:
                    parseStar();
                    break;
                case Token::GreaterThan:
                    parseGreatherThan();
                    break;
                case Token::PercentageUnit:
                case Token::PixelUnit:
                    parseUnit();
                    break;
                case Token::OpeningParenthesis:
                    parseOpeningParenthesis();
                    break;
                case Token::ClosingParenthesis:
                    parseClosingParenthesis();
                    break;
                case Token::OpeningCurlyBracket:
                    parseOpeningCurlyBracket();
                    break;
                case Token::ClosingCurlyBracket:
                    parseClosingCurlyBracket();
                    break;
                default:
                    throw UnknownToken(*currentNode);
                }
#ifdef DEBUG
                std::cerr << "Root :\n";
                expressionTreeRoot->display(std::cerr);
                std::cerr << "\n";
#endif
                currentNode = currentNode->getNext();
            }
            removeWhiteSpaces();
#ifdef DEBUG
            std::cerr << "Final parsed tree :\n";
            expressionTreeRoot->display(std::cerr);
            std::cerr << "\n";
#endif
            if (parsedTree != expressionTreeRoot) throw MalformedExpression("Block not properly closed\n");
        }
        catch (const ParserException &) {
            parsedTree = nullptr;
            delete expressionTreeRoot;
            expressionTreeRoot = nullptr;
            throw;
        }
    }

    void Parser::parseSpace() {
        Node *lastChild;
        if (parsedTree->getToken() == Token::Declaration) {
            if (parsedTree->getNbChilds() > 0) {
                lastChild = parsedTree->getLastChild();
                if (lastChild == nullptr || !isComponentRelation(lastChild->getToken())) parsedTree->appendChild(new Node(Token::AnyParent));
            }
        }
        else parsedTree->appendChild(currentNode->copyNode());
    }

    void Parser::parseLineBreak() {
        Token token = parsedTree->getToken();
        if (token != Token::NullRoot && token != Token::BlockDefinition) {
            throw MalformedExpression("A line break can only be between blocks declarations or assignments");
        }
        parsedTree->appendChild(currentNode->copyNode());
    }

    void Parser::parseOneLineComment() {}

    void Parser::parseMultiLineComment() {}

    void Parser::parseValue() {
        removeSpace();

        Node *lastChild;
        if (parsedTree->getToken() != Token::Assignment) {
            if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function)
                throw MalformedExpression(
                    "An int|float|bool|string must follow an assignment symbol or be inside of a tuple or a function parameter");
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(currentNode->copyNode());
        }
        else {
            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            parsedTree->appendChild(currentNode->copyNode());
        }
    }

    void Parser::parseComma() {
        removeSpace();
        switch (parsedTree->getToken()) {
        case Token::Tuple:
            parsedTree->appendChild(new Node(Token::ArgSeparator));
            break;
        case Token::Declaration:
            parsedTree = parsedTree->getParent()->appendChild(new Node(Token::Declaration));
            break;
        default:
            throw MalformedExpression("A comma must separate blocks definitions or be inside of a tuple");
        }
    }

    void Parser::parseColon() {
        removeSpace();

        Node *lastChild = parsedTree->getLastChild();
        Node *newChild;
        if (parsedTree->getToken() == Token::BlockDefinition && lastChild != nullptr && lastChild->getToken() == Token::Name) {
            lastChild->setToken(Token::RuleName);
            newChild = new Node(Token::Assignment);
            newChild->appendChild(lastChild->copyNodeWithChilds());
            parsedTree->replaceChild(lastChild, newChild);
            parsedTree = newChild;
        }
        else if (currentNode->getNext()->getToken() == Token::RawName) {
            currentNode = currentNode->getNext();
            parseModifier();
        }
        else throw MalformedExpression("A colon must be inside of a style block");
    }

    void Parser::parseSemiColon() {
        removeSpace();
        if ((parsedTree->getToken() == Token::Assignment && parsedTree->getNbChilds() > 1) || parsedTree->getToken() == Token::Import)
            parsedTree = parsedTree->getParent();
        else throw MalformedExpression("A semi-colon must be at the end of an assignment");
    }

    void Parser::parseSharp() {
        Node *lastChild;
        removeSpace();
        currentNode = currentNode->getNext();
        if (parsedTree->getToken() != Token::Assignment) {
            if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function) {
                parseIdentifier();
                return;
            }
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator) {
                parseIdentifier();
                return;
            }
            if (currentNode->getToken() != Token::RawName && currentNode->getToken() != Token::Int) return;
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(new Node{Token::Hex, currentNode->getValue()});
        }
        else {
            if (currentNode->getToken() != Token::RawName && currentNode->getToken() != Token::Int) return;
            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            parsedTree->appendChild(new Node{Token::Hex, currentNode->getValue()});
        }
    }

    void Parser::parseDot() {
        removeSpace();
        if (currentNode->getNext()->getToken() == Token::RawName) {
            currentNode = currentNode->getNext();
            parseClass();
        }
        else throw MalformedExpression("Illegal '.' placement");
    }

    void Parser::parseAmpersand() {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->getToken())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->getToken() == Token::Name) lastChildCopy = new Node{Token::ElementName, lastChild->getValue()};
                else if (lastChild->getToken() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '&' token must not be the first token of a block declaration if not a nested block");
            }
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node(Token::SameElement));
        }
        else if (token == Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild->getToken() == Token::AnyParent) {
                parsedTree->deleteSpecificChild(lastChild);
            }
            parsedTree->appendChild(new Node(Token::SameElement));
        }
        else
            throw MalformedExpression("A same element relation ('&') must be before a style block opening and at the root level of the style file or "
                                      "inside an other style block");
    }

    void Parser::parseAt() {
        if (parsedTree->getToken() != Token::NullRoot) throw MalformedExpression("A '@' (at) token must be on the root level");
        currentNode = currentNode->getNext();
        if (currentNode == nullptr) throw MalformedExpression("A '@' (at) token must not be alone");
        if (currentNode->getToken() == Token::RawName && currentNode->getValue() == "import")
            parsedTree = parsedTree->appendChild(new Node{Token::Import});
        else throw MalformedExpression("Invalid '@' (at) placement");
    }

    void Parser::parseStar() {
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            removeWhiteSpaces();
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
        }
        else if (token == Token::Declaration) {
            removeSpace();
        }
        else return;
        parsedTree->appendChild(new Node(Token::StarWildcard));
    }

    void Parser::parseString() {
        if (parsedTree->getToken() == Token::Import) {
            if (parsedTree->getLastChild()->getToken() != Token::Space)
                throw MalformedExpression("A space is needed between '@import' and the file name");
            removeSpace();
            parsedTree->setValue(currentNode->getValue());
        }
        else parseValue();
    }

    void Parser::parseGreatherThan() {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->getToken())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->getToken() == Token::Name) lastChildCopy = new Node{Token::ElementName, lastChild->getValue()};
                else if (lastChild->getToken() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '>' token must not be the first token of a block declaration if not a nested block");
            }
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node(Token::DirectParent));
        }
        else if (token == Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild->getToken() == Token::AnyParent) {
                parsedTree->deleteSpecificChild(lastChild);
            }
            parsedTree->appendChild(new Node(Token::DirectParent));
        }
        else
            throw MalformedExpression(
                "A direct parent relation ('>') must be before a style block opening and at the root level of the style file or "
                "inside an other style block");
    }

    void Parser::parseOpeningParenthesis() {
        removeSpace();

        Node *lastChild;
        if (parsedTree->getToken() == Token::Assignment) {
            if (parsedTree->getNbChilds() > 1) {
                lastChild = parsedTree->getLastChild();
                if (lastChild != nullptr && lastChild->getToken() == Token::Name) {
                    parsedTree->replaceChild(lastChild, new Node{Token::Function, lastChild->getValue()});
                }
                else throw MalformedExpression("A tuple must be the only right value of an assignment");
            }
            else parsedTree = parsedTree->appendChild(new Node(Token::Tuple));
            return;
        }
        if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function)
            throw MalformedExpression("A tuple|function must follow an assignment symbol or be a function parameter or inside of a tuple");
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr) {
            if (lastChild->getToken() == Token::Name) {
                lastChild->setToken(Token::Function);
                parsedTree = lastChild;
                return;
            }
            else if (lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        }
        parsedTree->deleteSpecificChild(lastChild);
        parsedTree = parsedTree->appendChild(new Node(Token::Tuple));
    }

    void Parser::parseClosingParenthesis() {
        removeSpace();

        if (parsedTree->getToken() != Token::Function && parsedTree->getToken() != Token::Tuple)
            throw MissingToken("A closing parenthesis ')' needs an opening parenthesis '('");
        parsedTree = parsedTree->getParent();
    }

    void Parser::parseOpeningCurlyBracket() {
        Node *lastChild;
        Node *lastChildCopy;
        removeSpace();
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::AnyParent) {
            parsedTree->deleteSpecificChild(lastChild);
        }
        if (parsedTree->getToken() != Token::NullRoot
            && parsedTree->getToken() != Token::BlockDefinition
            && parsedTree->getToken() != Token::Declaration)
            throw MalformedExpression("A style block must be defined in an other style block or at the root level of the file");
        if (parsedTree->getToken() != Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild == nullptr)
                throw MalformedExpression("A style block must start with at list an element name|class|identifier before the opening curly bracket");
            lastChildCopy = lastChild->copyNodeWithChilds();
            if (lastChildCopy->getToken() == Token::Name) lastChildCopy->setToken(Token::ElementName);
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
        }
        parsedTree = parsedTree->getParent()->getParent()->appendChild(new Node(Token::BlockDefinition));
    }

    void Parser::parseClosingCurlyBracket() {
        removeWhiteSpaces();
        if (parsedTree->getToken() == Token::Assignment) throw MissingToken("Missing semi-colon after assignment");
        Node *lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() != Token::Assignment && lastChild->getToken() != Token::StyleBlock)
            throw MalformedExpression("A block definition must only contains assgnments and other blocks");
        else if (parsedTree->getToken() != Token::BlockDefinition)
            throw MissingToken("A closing curly bracket '}' needs an opening curly bracket '{'");
        parsedTree = parsedTree->getParent()->getParent();
    }

    void Parser::parseRawName() {
        Node *lastChild;
        if (parsedTree->getToken() == Token::Assignment) {
            removeSpace();

            if (parsedTree->getNbChilds() != 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            parsedTree->appendChild(new Node{Token::EnumValue, currentNode->getValue()});
        }
        else if (parsedTree->getToken() == Token::Tuple || parsedTree->getToken() == Token::Function) {
            removeSpace();

            //FIXME: wtf? With that, `test-rule: (aaa, bbb, ccc);` doesn't work.
            // But, can tuples have enums inside?
            if (parsedTree->getNbChilds() != 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(new Node{Token::RawName, parsedTree->getValue()});
        }
        else {
            if (isValidElementOrRuleName(currentNode->getValue())) parseName();
            else throw MalformedExpression("Illegal raw name placement");
        }
    }

    void Parser::parseName() {
        Token token = parsedTree->getToken();
        Node *lastChild;
        Node *lastChildCopy;
        if (token == Token::NullRoot) {
            removeWhiteSpaces();

            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(new Node{Token::ElementName, currentNode->getValue()});
            return;
        }
        if (token == Token::BlockDefinition) {
            removeWhiteSpaces();
            lastChild = parsedTree->getLastChild();

            if (lastChild != nullptr && lastChild->getToken() == Token::RawName) {
                lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
                parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                                 ->appendChild(new Node(Token::BlockDeclaration))
                                 ->appendChild(new Node(Token::Declaration));
                parsedTree->appendChild(lastChildCopy);
                parsedTree->appendChild(new Node{Token::ElementName, currentNode->getValue()});
            }
            else parsedTree->appendChild(new Node{Token::Name, currentNode->getValue()});
            return;
        }

        if (token == Token::Assignment) {
            removeSpace();

            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("A string|function must be the only right value of an assignment");
            parsedTree->appendChild(new Node{Token::Name, currentNode->getValue()});
            return;
        }
        if (token == Token::Declaration) {
            parsedTree->appendChild(new Node{Token::ElementName, currentNode->getValue()});
            return;
        }
        removeSpace();

        if (token != Token::Tuple && token != Token::Function)
            throw MalformedExpression("A string|function must follow an assignment symbol or be inside of a tuple or a function parameter");
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
            throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        parsedTree->deleteSpecificChild(lastChild);
        parsedTree->appendChild(currentNode->copyNode());
    }

    void Parser::parseUnit() {
        Node *lastChild;
        Node *newChild;
        if (parsedTree->getToken() != Token::Assignment && parsedTree->getToken() != Token::Function && parsedTree->getToken() != Token::Tuple)
            throw MalformedExpression("An unit must be inside an assignment, a function or a tuple");

        lastChild = parsedTree->getLastChild();
        if (lastChild == nullptr || (lastChild->getToken() != Token::Int && lastChild->getToken() != Token::Float))
            throw MissingToken("A unit must have an int or a float before");
        newChild = new Node{currentNode->getToken(), currentNode->getValue()};
        newChild->appendChild(lastChild->copyNodeWithChilds());
        parsedTree->replaceChild(lastChild, newChild);
    }

    Node *Parser::updateLastDeclarationComponentBeforeNewOne(Node *lastChild) {
        Node *finalChild = nullptr;
        if (lastChild != nullptr) {
            if (isWhiteSpace(lastChild->getToken())) {
                removeWhiteSpaces();
                lastChild = nullptr;
            }
            else if (lastChild->getToken() == Token::Name) finalChild = new Node{Token::ElementName, lastChild->getValue()};
            else finalChild = lastChild->copyNodeWithChilds();
            parsedTree->deleteSpecificChild(lastChild);
        }
        return finalChild;
    }

    void Parser::parseDeclarationComponent(Token outputTokenType) {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            lastChildCopy = updateLastDeclarationComponentBeforeNewOne(lastChild);
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock))
                             ->appendChild(new Node(Token::BlockDeclaration))
                             ->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node{outputTokenType, currentNode->getValue()});
        }
        else if (token == Token::Declaration) {
            parsedTree->appendChild(new Node{outputTokenType, currentNode->getValue()});
        }
        else
            throw MalformedExpression(
                "A "
                + tokenToString(outputTokenType)
                + " must be before a style block opening and at the root level of the style file or inside an other style block");
    }

    void Parser::parseClass() { parseDeclarationComponent(Token::Class); }

    void Parser::parseIdentifier() { parseDeclarationComponent(Token::Identifier); }

    void Parser::parseModifier() { parseDeclarationComponent(Token::Modifier); }

} // namespace style
