#include "parser.hpp"
#include "deserialization_node.hpp"
#include "tokens.hpp"
#include <cctype>
#include <string>
#include <vector>

namespace style::parser {
    std::string parsingBlockToString(ParsingBlock block) {
        switch (block) {
        case ParsingBlock::SELECTORS:
            return "Selectors";
        case ParsingBlock::FILE:
            return "File";
        default:
            return "Unknown";
        }
    }

    std::string errorTypeToString(ErrorType type) {
        switch (type) {
        case ErrorType::WARNING:
            return "Error";
        case ErrorType::ERROR:
            return "Error";
        default:
            return "Unknown";
        }
    }

    namespace {
        std::list<ErrorMessage> *createErrorList(ErrorType type, std::string functionName, std::string message) {
            return new std::list<ErrorMessage>({{type, functionName + ": " + message}});
        }

        /*
         * methods in the parser returning an instance of this structure should always set currentLexedNode.
         * If currentLexedNode is null, it should mean the parsing method has read all nodes.
         */
        struct ParsingState {
            const DeserializationNode *currentLexedNode = nullptr;
            DeserializationNode *currentParsedNode = nullptr;
            std::list<ErrorMessage> *errors = nullptr;
        };

        bool isValidName(const std::string &str, size_t start, size_t end) {
            size_t i;
            for (i = start; i < end - 1; i++) {
                if (!isalpha(str[i]) && str[i] != '_' && str[i] != '-') return false;
            }
            if (!isalpha(str[i]) && str[i] != '_') return false;
            return true;
        }
    }

    bool isValidElementOrRuleName(const std::string &str) {
        if (!isalpha(str[0])) return false;
        return str.size() == 1 || isValidName(str, 1, str.size());
    }

    namespace {

        bool isValidHex(const std::string &str) {
            if (str.size() != 3 && str.size() != 6 && str.size() != 8) return false;
            for (unsigned char character : str) {
                if (!std::isxdigit(character)) return false;
            }
            return true;
        }

        /* return true if node is not null and node's token is equal to given token
         * Safe to call with null node
         */
        bool isTokenIn(const DeserializationNode *node, std::vector<Token> tokens) {
            if (!node) return false;
            for (Token token : tokens) {
                if (node->token() == token) return true;
            }
            return tokens.empty();
        }

        // return true if both lexer and parser nodes are null
        bool isNull(const ParsingState &parsingState) { return !parsingState.currentLexedNode && !parsingState.currentParsedNode; }

        // return true if either lexer or parser node is null
        bool isAnyNull(const ParsingState &parsingState) { return !parsingState.currentLexedNode || !parsingState.currentParsedNode; }

        // TODO: add tab support
        /* remove spaces
         * Safe to call with null node
         */
        ParsingState removeSpaces(const DeserializationNode *currentLexedNode) {
            const DeserializationNode *startNode = currentLexedNode;
            const DeserializationNode *currentNode = startNode;
            while (isTokenIn(currentNode, {Token::Space})) {
                currentNode = currentNode->next();
            }
            if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{currentNode, nullptr};
        }

        /* remove spaces line breaks
         * Safe to call with null node
         */
        ParsingState removeWhiteSpaces(const DeserializationNode *currentLexedNode) {
            const DeserializationNode *startNode = currentLexedNode;
            const DeserializationNode *currentNode = startNode;
            while (isTokenIn(currentNode, {Token::Space, Token::LineBreak})) {
                currentNode = currentNode->next();
            }
            if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{currentNode, nullptr};
        }

        /* Safe to call with null node
         */
        ParsingState removeComments(const DeserializationNode *currentLexedNode) {
            if (isTokenIn(currentLexedNode, {Token::OneLineComment, Token::MultiLineComment})) currentLexedNode = currentLexedNode->next();
            return ParsingState{currentLexedNode, nullptr};
        }

        /* Safe to call with null node
         */
        ParsingState removeWhiteSpacesAndComment(const DeserializationNode *currentLexedNode) {
            ParsingState state = {currentLexedNode};
            const DeserializationNode *previousLexedNode;
            do {
                previousLexedNode = state.currentLexedNode;
                state = removeWhiteSpaces(state.currentLexedNode);
                state = removeComments(state.currentLexedNode);
            } while (state.currentLexedNode && state.currentLexedNode != previousLexedNode);
            return ParsingState{state.currentLexedNode, nullptr};
        }

        ParsingState tryParseElementName(const DeserializationNode *currentLexedNode) {
            if (currentLexedNode->token() == Token::RawName && isValidElementOrRuleName(currentLexedNode->value())) {
                return ParsingState{currentLexedNode->next(), new DeserializationNode(Token::ElementName, currentLexedNode->value())};
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseClass(const DeserializationNode *currentLexedNode) {
            if (currentLexedNode->token() == Token::Dot) {
                const DeserializationNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new DeserializationNode(Token::Class, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseIdentifier(const DeserializationNode *currentLexedNode) {
            if (currentLexedNode->token() == Token::Sharp) {
                const DeserializationNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new DeserializationNode(Token::Identifier, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseModifier(const DeserializationNode *currentLexedNode) {
            if (currentLexedNode->token() == Token::Colon) {
                const DeserializationNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new DeserializationNode(Token::Modifier, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseSelector(const DeserializationNode *currentLexedNode) {
            ParsingState selectorParsingState = tryParseElementName(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseClass(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseIdentifier(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseModifier(currentLexedNode);
            return selectorParsingState;
        }

        ParsingState tryParseDirectParentRelation(const DeserializationNode *currentLexedNode) {
            ParsingState state = removeWhiteSpacesAndComment(currentLexedNode);
            if (state.currentLexedNode->token() != Token::GreaterThan) return {};
            state.currentLexedNode = state.currentLexedNode->next();
            state.currentParsedNode = new DeserializationNode(Token::DirectParent);
            return state;
        }

        ParsingState tryParseAnyParentRelation(const DeserializationNode *currentLexedNode) {
            ParsingState state = removeWhiteSpacesAndComment(currentLexedNode);
            if (state.currentLexedNode != currentLexedNode) return {state.currentLexedNode, new DeserializationNode(Token::AnyParent)};
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseSelectorsRelation(const DeserializationNode *currentLexedNode) {
            ParsingState state = tryParseDirectParentRelation(currentLexedNode);
            if (!state.currentParsedNode) state = tryParseAnyParentRelation(currentLexedNode);
            return state;
        }

        ParsingState tryParseSelectorsList(const DeserializationNode *currentLexedNodeStart) {
            const DeserializationNode *currentLexedNode = currentLexedNodeStart;
            ParsingState state;
            ParsingState selectorsRelationState;

            DeserializationNode *currentParsedNode = new DeserializationNode{Token::SelectorsList};

            do {
                state = removeSpaces(selectorsRelationState.currentParsedNode ? selectorsRelationState.currentLexedNode : currentLexedNode);
                if (!state.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};

                // parse selectors with no separator between
                state = tryParseSelector(state.currentLexedNode);
                if (!state.currentParsedNode) break;
                if (selectorsRelationState.currentParsedNode) currentParsedNode->addChild(selectorsRelationState.currentParsedNode);
                if (!state.currentLexedNode) break;

                currentLexedNode = state.currentLexedNode;
                currentParsedNode->addChild(state.currentParsedNode);

                state = tryParseSelector(currentLexedNode);
                while (state.currentParsedNode && currentLexedNode) {
                    currentParsedNode->addChild(new DeserializationNode{Token::SameElement});
                    currentLexedNode = state.currentLexedNode;
                    currentParsedNode->addChild(state.currentParsedNode);
                    state = tryParseSelector(currentLexedNode);
                }

                // end of lexed list reached
                if (!currentLexedNode) break;

                selectorsRelationState = tryParseSelectorsRelation(currentLexedNode);
            } while (selectorsRelationState.currentLexedNode && selectorsRelationState.currentParsedNode);

            if (!currentParsedNode->nbChilds()) {
                delete currentParsedNode;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    new std::list<ErrorMessage>({{ErrorType::ERROR, "tryParseSelectorsList: No selectors found"}})};
            }
            return ParsingState{currentLexedNode, currentParsedNode};
        }

        ParsingState tryParseSelectorsBlock(const DeserializationNode *currentLexedNodeStart) {
            const DeserializationNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::SelectorsBlock};
            while (true) {
                ParsingState state = tryParseSelectorsList(currentLexedNode);
                if (!state.currentParsedNode) {
                    delete currentParsedNodeRoot;
                    return ParsingState{currentLexedNodeStart, nullptr};
                }
                if (!state.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
                currentParsedNodeRoot->addChild(state.currentParsedNode);
                removedSpacesState = removeSpaces(state.currentLexedNode);
                if (!removedSpacesState.currentLexedNode || removedSpacesState.currentLexedNode->token() != Token::Comma) {
                    return ParsingState{state.currentLexedNode, currentParsedNodeRoot};
                }
                currentLexedNode = removedSpacesState.currentLexedNode->next(); // skipping comma
            }
        }

        ParsingState tryParseRuleName(const DeserializationNode *currentLexedNode) {
            ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::RawName})) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{removedSpacesState.currentLexedNode->next(),
                                new DeserializationNode{Token::RuleName, removedSpacesState.currentLexedNode->value()}};
        }

        ParsingState tryParseRuleValue(const DeserializationNode *currentLexedNode) {
            DeserializationNode *node = nullptr;
            DeserializationNode *unit = nullptr;
            ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNode, nullptr};
            const DeserializationNode *value = removedSpacesState.currentLexedNode;

            switch (value->token()) {
            case Token::Int:
                node = new DeserializationNode{Token::Int, value->value()};
                value = value->next();
                if (value && value->token() == Token::Unit) {
                    unit = new DeserializationNode{Token::Unit, value->value()};
                    unit->addChild(node);
                    node = unit;
                    value = value->next();
                }
                return ParsingState{value, node};
            case Token::Float:
                node = new DeserializationNode{Token::Float, value->value()};
                value = value->next();
                if (value && value->token() == Token::Unit) {
                    unit = new DeserializationNode{Token::Unit, value->value()};
                    unit->addChild(node);
                    node = unit;
                    value = value->next();
                }
                return ParsingState{value, node};
            case Token::Bool:
                return ParsingState{value->next(), new DeserializationNode{Token::Bool, value->value()}};
            case Token::Sharp:
                if (isTokenIn(value->next(), {Token::RawName, Token::Int}) && isValidHex(value->next()->value())) {
                    return ParsingState{value->next()->next(), new DeserializationNode{Token::Hex, value->next()->value()}};
                }
                return ParsingState{currentLexedNode, nullptr};
            default:
                return ParsingState{currentLexedNode, nullptr};
            }
        }

        ParsingState tryParseRuleAssignment(const DeserializationNode *currentLexedNodeStart) {
            const DeserializationNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            ParsingState state;
            DeserializationNode *assignment = new DeserializationNode{Token::Assignment};

            state = tryParseRuleName(currentLexedNode);
            if (isAnyNull(state)) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "tryParseRuleAssignment", "Missing a rule name")};
            }
            assignment->addChild(state.currentParsedNode);

            removedSpacesState = removeWhiteSpacesAndComment(state.currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::Colon})) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr, createErrorList(ErrorType::ERROR, "tryParseRuleAssignment", "Missing a colon")};
            }

            removedSpacesState = removeWhiteSpacesAndComment(removedSpacesState.currentLexedNode->next());
            state = tryParseRuleValue(removedSpacesState.currentLexedNode);
            if (isAnyNull(state)) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "tryParseRuleAssignment", "Missing a rule value")};
            }
            assignment->addChild(state.currentParsedNode);

            removedSpacesState = removeWhiteSpacesAndComment(state.currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::SemiColon})) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "tryParseRuleAssignment", "Missing a semi colon")};
            }
            return ParsingState{removedSpacesState.currentLexedNode->next(), assignment};
        }

        ParsingState tryParseRulesBlock(const DeserializationNode *currentLexedNodeStart) {
            const DeserializationNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            ParsingState state;

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::OpeningCurlyBracket})) return ParsingState{currentLexedNodeStart, nullptr};
            currentLexedNode = removedSpacesState.currentLexedNode->next();

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};
            currentLexedNode = removedSpacesState.currentLexedNode;

            DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::BlockDeclarations};

            while (true) {
                state = tryParseRuleAssignment(currentLexedNode);
                if (!state.currentParsedNode) break;
                currentParsedNodeRoot->addChild(state.currentParsedNode);
                currentLexedNode = state.currentLexedNode;
                removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
                if (removedSpacesState.currentLexedNode) currentLexedNode = removedSpacesState.currentLexedNode;
            }

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::ClosingCurlyBracket})) {
                delete currentParsedNodeRoot;

                if (state.errors) { // if there was errors during the parsing of a rule assignment, and the current token does not mean end of rule
                                    // block, then it's a malformed text
                    return ParsingState{currentLexedNodeStart, nullptr, state.errors};
                }
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "tryParseRulesBlock", "Missing a closing curly bracket")};
            }
            return ParsingState{removedSpacesState.currentLexedNode->next(), currentParsedNodeRoot};
        }

        ParsingState tryParseImport(const DeserializationNode *currentLexedNode) { return ParsingState{currentLexedNode, nullptr}; }

        ParsingState tryParseAtRule(const DeserializationNode *currentLexedNode) { return ParsingState{currentLexedNode, nullptr}; }

        ParsingState tryParseFile(const DeserializationNode *currentLexedNodeStart) {
            ParsingState state;

            state = removeWhiteSpacesAndComment(currentLexedNodeStart);
            if (isNull(state)) return ParsingState{currentLexedNodeStart, nullptr};

            state = tryParseSelectorsBlock(state.currentLexedNode);
            if (!state.currentLexedNode || !state.currentParsedNode) return ParsingState{currentLexedNodeStart, nullptr};

            DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::StyleBlock};
            currentParsedNodeRoot->addChild(state.currentParsedNode);

            state = tryParseRulesBlock(state.currentLexedNode);
            if (!state.currentParsedNode) {
                delete currentParsedNodeRoot;
                return ParsingState{currentLexedNodeStart, nullptr, state.errors};
            }

            currentParsedNodeRoot->addChild(state.currentParsedNode);

            return ParsingState{state.currentLexedNode, currentParsedNodeRoot};
        }
    }

    ParseResult parse(DeserializationNode *currentNode, ParsingBlock block) {
        std::list<ErrorMessage> *errorMessages = new std::list<ErrorMessage>();

        ParsingState finalState;
        switch (block) {
        case ParsingBlock::FILE:
            finalState = tryParseFile(currentNode);
            break;
        case ParsingBlock::SELECTORS:
            finalState = tryParseSelectorsBlock(currentNode);
            break;
        }

        if (finalState.errors) {
            errorMessages->splice(errorMessages->begin(), *finalState.errors);
        }

        if (!finalState.currentParsedNode) {
            errorMessages->push_back({ErrorType::ERROR, "parse: Can't deserialize (" + parsingBlockToString(block) + ")"});
        }

        DeserializationNode *rootNode = new DeserializationNode(Token::NullRoot);
        rootNode->addChild(finalState.currentParsedNode);

#ifdef DEBUG
        std::clog << "Final parsed tree :\n";
        rootNode->debugDisplay(std::clog);
        std::clog << "\n";
#endif

        return {rootNode, errorMessages};
    }
} // namespace style
