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
        DeserializationNode *lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->token() == Token::Space) _parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeLineReturn() {
        DeserializationNode *lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->token() == Token::LineBreak) _parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeWhiteSpaces() {
        DeserializationNode *lastChild = _parsedTree->getLastChild();
        while (lastChild != nullptr && (lastChild->token() == Token::Space || lastChild->token() == Token::LineBreak)) {
            _parsedTree->deleteSpecificChild(lastChild);
            lastChild = _parsedTree->getLastChild();
        }
    }

    DeserializationNode *Parser::parse(DeserializationNode *currentNode) {
        _currentNode = currentNode;
        _expressionTreeRoot = new DeserializationNode(Token::NullRoot);
        _parsedTree = _expressionTreeRoot;
        try {
            while (_currentNode != nullptr) {
#ifdef DEBUG
                std::cerr << "\nActual token : " << tokenToString(_currentNode->token()) << ": '" << _currentNode->value() << "'" << "\n";
#endif
                switch (_currentNode->token()) {
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
                case Token::Unit:
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
                    throw UnknownToken(*_currentNode);
                }
#ifdef DEBUG
                std::cerr << "Root :\n";
                _expressionTreeRoot->debugDisplay(std::cerr);
                std::cerr << "\n";
#endif
                _currentNode = _currentNode->next();
            }
            removeWhiteSpaces();
#ifdef DEBUG
            std::cerr << "Final parsed tree :\n";
            _expressionTreeRoot->debugDisplay(std::cerr);
            std::cerr << "\n";
#endif
            if (_parsedTree != _expressionTreeRoot) throw MalformedExpression("Block not properly closed\n");
        }
        catch (const ParserException &) {
            _parsedTree = nullptr;
            delete _expressionTreeRoot;
            _expressionTreeRoot = nullptr;
            throw;
        }
        return _expressionTreeRoot;
    }

    void Parser::parseSpace() {
        DeserializationNode *lastChild;
        if (_parsedTree->token() == Token::Declaration) {
            if (_parsedTree->nbChilds() > 0) {
                lastChild = _parsedTree->getLastChild();
                if (lastChild == nullptr || !isComponentRelation(lastChild->token()))
                    _parsedTree->addChild(new DeserializationNode(Token::AnyParent));
            }
        }
        else _parsedTree->addChild(_currentNode->copyNode());
    }

    void Parser::parseLineBreak() {
        Token token = _parsedTree->token();
        if (token != Token::NullRoot && token != Token::BlockContent) {
            throw MalformedExpression("A line break can only be between blocks declarations or assignments");
        }
        _parsedTree->addChild(_currentNode->copyNode());
    }

    void Parser::parseOneLineComment() {}

    void Parser::parseMultiLineComment() {}

    void Parser::parseValue() {
        removeSpace();

        DeserializationNode *lastChild;
        if (_parsedTree->token() != Token::Assignment) {
            if (_parsedTree->token() != Token::Tuple && _parsedTree->token() != Token::Function)
                throw MalformedExpression(
                    "An int|float|bool|string must follow an assignment symbol or be inside of a tuple or a function parameter");
            lastChild = _parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->token() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            _parsedTree->deleteSpecificChild(lastChild);
            _parsedTree->addChild(_currentNode->copyNode());
        }
        else {
            if (_parsedTree->nbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            _parsedTree->addChild(_currentNode->copyNode());
        }
    }

    void Parser::parseComma() {
        removeSpace();
        switch (_parsedTree->token()) {
        case Token::Tuple:
            _parsedTree->addChild(new DeserializationNode(Token::ArgSeparator));
            break;
        case Token::Declaration:
            _parsedTree = _parsedTree->parent()->addChild(new DeserializationNode(Token::Declaration));
            break;
        default:
            throw MalformedExpression("A comma must separate blocks definitions or be inside of a tuple");
        }
    }

    void Parser::parseColon() {
        removeSpace();

        DeserializationNode *lastChild = _parsedTree->getLastChild();
        DeserializationNode *newChild;
        if (_parsedTree->token() == Token::BlockContent && lastChild != nullptr && lastChild->token() == Token::Name) {
            lastChild->token(Token::RuleName);
            newChild = new DeserializationNode(Token::Assignment);
            newChild->addChild(lastChild->copyNodeWithChilds());
            _parsedTree->replaceChild(lastChild, newChild);
            _parsedTree = newChild;
        }
        else if (_currentNode->next()->token() == Token::RawName) {
            _currentNode = _currentNode->next();
            parseModifier();
        }
        else throw MalformedExpression("A colon must be inside of a style block");
    }

    void Parser::parseSemiColon() {
        removeSpace();
        if ((_parsedTree->token() == Token::Assignment && _parsedTree->nbChilds() > 1) || _parsedTree->token() == Token::Import)
            _parsedTree = _parsedTree->parent();
        else throw MalformedExpression("A semi-colon must be at the end of an assignment");
    }

    void Parser::parseSharp() {
        DeserializationNode *lastChild;
        removeSpace();
        _currentNode = _currentNode->next();
        if (_parsedTree->token() != Token::Assignment) {
            if (_parsedTree->token() != Token::Tuple && _parsedTree->token() != Token::Function) {
                parseIdentifier();
                return;
            }
            lastChild = _parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->token() != Token::ArgSeparator) {
                parseIdentifier();
                return;
            }
            if (_currentNode->token() != Token::RawName && _currentNode->token() != Token::Int) return;
            _parsedTree->deleteSpecificChild(lastChild);
            _parsedTree->addChild(new DeserializationNode{Token::Hex, _currentNode->value()});
        }
        else {
            if (_currentNode->token() != Token::RawName && _currentNode->token() != Token::Int) return;
            if (_parsedTree->nbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            _parsedTree->addChild(new DeserializationNode{Token::Hex, _currentNode->value()});
        }
    }

    void Parser::parseDot() {
        removeSpace();
        if (_currentNode->next()->token() == Token::RawName) {
            _currentNode = _currentNode->next();
            parseClass();
        }
        else throw MalformedExpression("Illegal '.' placement");
    }

    void Parser::parseAmpersand() {
        DeserializationNode *lastChild;
        DeserializationNode *lastChildCopy = nullptr;
        Token token = _parsedTree->token();
        if (token == Token::NullRoot || token == Token::BlockContent) {
            lastChild = _parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->token())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->token() == Token::Name) lastChildCopy = new DeserializationNode{Token::ElementName, lastChild->value()};
                else if (lastChild->token() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                _parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '&' token must not be the first token of a block declaration if not a nested block");
            }
            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
            _parsedTree->addChild(lastChildCopy);
            _parsedTree->addChild(new DeserializationNode(Token::SameElement));
        }
        else if (token == Token::Declaration) {
            lastChild = _parsedTree->getLastChild();
            if (lastChild->token() == Token::AnyParent) {
                _parsedTree->deleteSpecificChild(lastChild);
            }
            _parsedTree->addChild(new DeserializationNode(Token::SameElement));
        }
        else
            throw MalformedExpression("A same element relation ('&') must be before a style block opening and at the root level of the style file or "
                                      "inside an other style block");
    }

    void Parser::parseAt() {
        if (_parsedTree->token() != Token::NullRoot) throw MalformedExpression("A '@' (at) token must be on the root level");
        _currentNode = _currentNode->next();
        if (_currentNode == nullptr) throw MalformedExpression("A '@' (at) token must not be alone");
        if (_currentNode->token() == Token::RawName && _currentNode->value() == "import") {
            removeWhiteSpaces();
            _parsedTree = _parsedTree->addChild(new DeserializationNode{Token::Import});
        }
        else throw MalformedExpression("Invalid '@' (at) placement");
    }

    void Parser::parseStar() {
        Token token = _parsedTree->token();
        if (token == Token::NullRoot || token == Token::BlockContent) {
            removeWhiteSpaces();
            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
        }
        else if (token == Token::Declaration) {
            removeSpace();
        }
        else return;
        _parsedTree->addChild(new DeserializationNode(Token::StarWildcard));
    }

    void Parser::parseString() {
        if (_parsedTree->token() == Token::Import) {
            if (_parsedTree->getLastChild()->token() != Token::Space)
                throw MalformedExpression("A space is needed between '@import' and the file name");
            removeSpace();
            _parsedTree->value(_currentNode->value());
        }
        else parseValue();
    }

    void Parser::parseGreatherThan() {
        DeserializationNode *lastChild;
        DeserializationNode *lastChildCopy = nullptr;
        Token token = _parsedTree->token();
        if (token == Token::NullRoot || token == Token::BlockContent) {
            lastChild = _parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->token())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->token() == Token::Name) lastChildCopy = new DeserializationNode{Token::ElementName, lastChild->value()};
                else if (lastChild->token() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                _parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '>' token must not be the first token of a block declaration if not a nested block");
            }
            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
            _parsedTree->addChild(lastChildCopy);
            _parsedTree->addChild(new DeserializationNode(Token::DirectParent));
        }
        else if (token == Token::Declaration) {
            lastChild = _parsedTree->getLastChild();
            if (lastChild->token() == Token::AnyParent) {
                _parsedTree->deleteSpecificChild(lastChild);
            }
            _parsedTree->addChild(new DeserializationNode(Token::DirectParent));
        }
        else
            throw MalformedExpression(
                "A direct parent relation ('>') must be before a style block opening and at the root level of the style file or "
                "inside an other style block");
    }

    void Parser::parseOpeningParenthesis() {
        removeSpace();

        DeserializationNode *lastChild;
        if (_parsedTree->token() == Token::Assignment) {
            if (_parsedTree->nbChilds() > 1) {
                lastChild = _parsedTree->getLastChild();
                if (lastChild != nullptr && lastChild->token() == Token::Name) {
                    _parsedTree->replaceChild(lastChild, new DeserializationNode{Token::Function, lastChild->value()});
                }
                else throw MalformedExpression("A tuple must be the only right value of an assignment");
            }
            else _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::Tuple));
            return;
        }
        if (_parsedTree->token() != Token::Tuple && _parsedTree->token() != Token::Function)
            throw MalformedExpression("A tuple|function must follow an assignment symbol or be a function parameter or inside of a tuple");
        lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr) {
            if (lastChild->token() == Token::Name) {
                lastChild->token(Token::Function);
                _parsedTree = lastChild;
                return;
            }
            else if (lastChild->token() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        }
        _parsedTree->deleteSpecificChild(lastChild);
        _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::Tuple));
    }

    void Parser::parseClosingParenthesis() {
        removeSpace();

        if (_parsedTree->token() != Token::Function && _parsedTree->token() != Token::Tuple)
            throw MissingToken("A closing parenthesis ')' needs an opening parenthesis '('");
        _parsedTree = _parsedTree->parent();
    }

    void Parser::parseOpeningCurlyBracket() {
        DeserializationNode *lastChild;
        DeserializationNode *lastChildCopy;
        removeSpace();
        lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->token() == Token::AnyParent) {
            _parsedTree->deleteSpecificChild(lastChild);
        }
        if (_parsedTree->token() != Token::NullRoot && _parsedTree->token() != Token::BlockContent && _parsedTree->token() != Token::Declaration)
            throw MalformedExpression("A style block must be defined in an other style block or at the root level of the file");
        if (_parsedTree->token() != Token::Declaration) {
            lastChild = _parsedTree->getLastChild();
            if (lastChild == nullptr)
                throw MalformedExpression("A style block must start with at list an element name|class|identifier before the opening curly bracket");
            lastChildCopy = lastChild->copyNodeWithChilds();
            if (lastChildCopy->token() == Token::Name) lastChildCopy->token(Token::ElementName);
            _parsedTree->deleteSpecificChild(lastChild);
            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
            _parsedTree->addChild(lastChildCopy);
        }
        _parsedTree = _parsedTree->parent()->parent()->addChild(new DeserializationNode(Token::BlockContent));
    }

    void Parser::parseClosingCurlyBracket() {
        removeWhiteSpaces();
        if (_parsedTree->token() == Token::Assignment) throw MissingToken("Missing semi-colon after assignment");
        DeserializationNode *lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->token() != Token::Assignment && lastChild->token() != Token::StyleBlock)
            throw MalformedExpression("A block content must only contains assignments and other blocks");
        else if (_parsedTree->token() != Token::BlockContent) throw MissingToken("A closing curly bracket '}' needs an opening curly bracket '{'");
        _parsedTree = _parsedTree->parent()->parent();
    }

    void Parser::parseRawName() {
        DeserializationNode *lastChild;
        if (_parsedTree->token() == Token::Assignment) {
            removeSpace();

            if (_parsedTree->nbChilds() != 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            _parsedTree->addChild(new DeserializationNode{Token::EnumValue, _currentNode->value()});
        }
        else if (_parsedTree->token() == Token::Tuple || _parsedTree->token() == Token::Function) {
            removeSpace();

            // FIXME: wtf? With that, `test-rule: (aaa, bbb, ccc);` doesn't work.
            //  But, can tuples have enums inside?
            if (_parsedTree->nbChilds() != 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            lastChild = _parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->token() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            _parsedTree->deleteSpecificChild(lastChild);
            _parsedTree->addChild(new DeserializationNode{Token::RawName, _parsedTree->value()});
        }
        else {
            if (isValidElementOrRuleName(_currentNode->value())) parseName();
            else throw MalformedExpression("Illegal raw name placement");
        }
    }

    void Parser::parseName() {
        Token token = _parsedTree->token();
        DeserializationNode *lastChild;
        DeserializationNode *lastChildCopy;
        if (token == Token::NullRoot) {
            removeWhiteSpaces();

            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
            _parsedTree->addChild(new DeserializationNode{Token::ElementName, _currentNode->value()});
            return;
        }
        if (token == Token::BlockContent) {
            removeWhiteSpaces();
            lastChild = _parsedTree->getLastChild();

            if (lastChild != nullptr && lastChild->token() == Token::RawName) {
                lastChildCopy = lastChild->copyNodeWithChilds();
                _parsedTree->deleteSpecificChild(lastChild);
                _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                                  ->addChild(new DeserializationNode(Token::BlockDeclaration))
                                  ->addChild(new DeserializationNode(Token::Declaration));
                _parsedTree->addChild(lastChildCopy);
                _parsedTree->addChild(new DeserializationNode{Token::ElementName, _currentNode->value()});
            }
            else _parsedTree->addChild(new DeserializationNode{Token::Name, _currentNode->value()});
            return;
        }

        if (token == Token::Assignment) {
            removeSpace();

            if (_parsedTree->nbChilds() > 1) throw MalformedExpression("A string|function must be the only right value of an assignment");
            _parsedTree->addChild(new DeserializationNode{Token::Name, _currentNode->value()});
            return;
        }
        if (token == Token::Declaration) {
            _parsedTree->addChild(new DeserializationNode{Token::ElementName, _currentNode->value()});
            return;
        }
        removeSpace();

        if (token != Token::Tuple && token != Token::Function)
            throw MalformedExpression("A string|function must follow an assignment symbol or be inside of a tuple or a function parameter");
        lastChild = _parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->token() != Token::ArgSeparator)
            throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        _parsedTree->deleteSpecificChild(lastChild);
        _parsedTree->addChild(_currentNode->copyNode());
    }

    void Parser::parseUnit() {
        DeserializationNode *lastChild;
        DeserializationNode *newChild;
        if (_parsedTree->token() != Token::Assignment && _parsedTree->token() != Token::Function && _parsedTree->token() != Token::Tuple)
            throw MalformedExpression("An unit must be inside an assignment, a function or a tuple");

        lastChild = _parsedTree->getLastChild();
        if (lastChild == nullptr || (lastChild->token() != Token::Int && lastChild->token() != Token::Float))
            throw MissingToken("A unit must have an int or a float before");
        newChild = new DeserializationNode{_currentNode->token(), _currentNode->value()};
        newChild->addChild(lastChild->copyNodeWithChilds());
        _parsedTree->replaceChild(lastChild, newChild);
    }

    DeserializationNode *Parser::updateLastDeclarationComponentBeforeNewOne(DeserializationNode *lastChild) {
        DeserializationNode *finalChild = nullptr;
        if (lastChild != nullptr) {
            if (isWhiteSpace(lastChild->token())) {
                removeWhiteSpaces();
                lastChild = nullptr;
            }
            else if (lastChild->token() == Token::Name) finalChild = new DeserializationNode{Token::ElementName, lastChild->value()};
            else finalChild = lastChild->copyNodeWithChilds();
            _parsedTree->deleteSpecificChild(lastChild);
        }
        return finalChild;
    }

    void Parser::parseDeclarationComponent(Token outputTokenType) {
        DeserializationNode *lastChild;
        DeserializationNode *lastChildCopy = nullptr;
        Token token = _parsedTree->token();
        if (token == Token::NullRoot || token == Token::BlockContent) {
            lastChild = _parsedTree->getLastChild();
            lastChildCopy = updateLastDeclarationComponentBeforeNewOne(lastChild);
            _parsedTree = _parsedTree->addChild(new DeserializationNode(Token::StyleBlock))
                              ->addChild(new DeserializationNode(Token::BlockDeclaration))
                              ->addChild(new DeserializationNode(Token::Declaration));
            _parsedTree->addChild(lastChildCopy);
            _parsedTree->addChild(new DeserializationNode{outputTokenType, _currentNode->value()});
        }
        else if (token == Token::Declaration) {
            _parsedTree->addChild(new DeserializationNode{outputTokenType, _currentNode->value()});
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
