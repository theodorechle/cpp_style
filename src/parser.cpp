#include "parser.hpp"
#include "lexer_node.hpp"
#include "lexer_tokens.hpp"
#include <cctype>
#include <list>
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
            return "Warning";
        case ErrorType::ERROR:
            return "Error";
        case ErrorType::LOG:
            return "Log";
        default:
            return "Unknown";
        }
    }

    std::string errorMessageToString(ErrorMessage message) { return message.message + " (" + errorTypeToString(message.type) + ")"; }

    namespace {
        // the firstBlockLexerNode is used to retrieve line and column and allow the caller not to handle null node
        std::list<ErrorMessage> *createErrorList(ErrorType type, std::string message, const lexer::LexerNode *firstBlockLexerNode,
                                                 std::list<ErrorMessage> *existingErrors = nullptr) {
            if (existingErrors == nullptr) existingErrors = new std::list<ErrorMessage>();
            if (firstBlockLexerNode) {
                existingErrors->push_back({type, message, firstBlockLexerNode->line(), firstBlockLexerNode->column()});
            }
            else existingErrors->push_back({type, message});
            return existingErrors;
        }

        std::list<ErrorMessage> *appendErrors(std::list<ErrorMessage> *existingErrors = nullptr, std::list<ErrorMessage> *newErrors = nullptr) {
            if (!existingErrors) return newErrors;
            if (!newErrors) return existingErrors;
            existingErrors->splice(existingErrors->cend(), *newErrors);
            delete newErrors;
            return existingErrors;
        }

        /*
         * methods in the parser returning an instance of this structure should always set currentLexedNode.
         * If currentLexedNode is null, it should mean the parsing method has read all nodes.
         */
        struct ParsingState {
            const lexer::LexerNode *currentLexedNode = nullptr;
            ParserNode *currentParsedNode = nullptr;
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
        bool isTokenIn(const lexer::LexerNode *node, std::vector<lexer::Token> tokens) {
            if (!node) return false;
            for (lexer::Token token : tokens) {
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
        ParsingState removeSpaces(const lexer::LexerNode *currentLexedNode) {
            const lexer::LexerNode *startNode = currentLexedNode;
            const lexer::LexerNode *currentNode = startNode;
            while (isTokenIn(currentNode, {lexer::Token::Space})) {
                currentNode = currentNode->next();
            }
            if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{currentNode, nullptr};
        }

        /* remove spaces line breaks
         * Safe to call with null node
         */
        ParsingState removeWhiteSpaces(const lexer::LexerNode *currentLexedNode) {
            const lexer::LexerNode *startNode = currentLexedNode;
            const lexer::LexerNode *currentNode = startNode;
            while (isTokenIn(currentNode, {lexer::Token::Space, lexer::Token::LineBreak})) {
                currentNode = currentNode->next();
            }
            if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{currentNode, nullptr};
        }

        /* Safe to call with null node
         */
        ParsingState removeComments(const lexer::LexerNode *currentLexedNode) {
            if (isTokenIn(currentLexedNode, {lexer::Token::OneLineComment, lexer::Token::MultiLineComment}))
                currentLexedNode = currentLexedNode->next();
            return ParsingState{currentLexedNode, nullptr};
        }

        /* Safe to call with null node
         */
        ParsingState removeWhiteSpacesAndComment(const lexer::LexerNode *currentLexedNode) {
            ParsingState state = {currentLexedNode};
            const lexer::LexerNode *previousLexedNode;
            do {
                previousLexedNode = state.currentLexedNode;
                state = removeWhiteSpaces(state.currentLexedNode);
                state = removeComments(state.currentLexedNode);
            } while (state.currentLexedNode && state.currentLexedNode != previousLexedNode);
            return ParsingState{state.currentLexedNode, nullptr};
        }

        ParsingState tryParseElementName(const lexer::LexerNode *currentLexedNode) {
            if (currentLexedNode->token() == lexer::Token::RawName && isValidElementOrRuleName(currentLexedNode->value())) {
                return ParsingState{currentLexedNode->next(), new ParserNode(Token::ElementName, currentLexedNode->value())};
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseClass(const lexer::LexerNode *currentLexedNode) {
            if (currentLexedNode->token() == lexer::Token::Dot) {
                const lexer::LexerNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {lexer::Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new ParserNode(Token::Class, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseIdentifier(const lexer::LexerNode *currentLexedNode) {
            if (currentLexedNode->token() == lexer::Token::Sharp) {
                const lexer::LexerNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {lexer::Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new ParserNode(Token::Identifier, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseModifier(const lexer::LexerNode *currentLexedNode) {
            if (currentLexedNode->token() == lexer::Token::Colon) {
                const lexer::LexerNode *nextNode = currentLexedNode->next();
                if (isTokenIn(nextNode, {lexer::Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                    return ParsingState{nextNode->next(), new ParserNode(Token::Modifier, nextNode->value())};
                }
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseWildcard(const lexer::LexerNode *currentLexedNode) {
            if (currentLexedNode->token() == lexer::Token::Star) {
                return ParsingState{currentLexedNode->next(), new ParserNode(Token::StarWildcard, currentLexedNode->value())};
            }
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseSelector(const lexer::LexerNode *currentLexedNode) {
            ParsingState selectorParsingState = tryParseElementName(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseClass(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseIdentifier(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseModifier(currentLexedNode);
            if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseWildcard(currentLexedNode);
            return selectorParsingState;
        }

        ParsingState tryParseDirectParentRelation(const lexer::LexerNode *currentLexedNode) {
            ParsingState state = removeWhiteSpacesAndComment(currentLexedNode);
            if (state.currentLexedNode->token() != lexer::Token::GreaterThan) return {};
            state.currentLexedNode = state.currentLexedNode->next();
            state.currentParsedNode = new ParserNode(Token::DirectParent);
            return state;
        }

        ParsingState tryParseAnyParentRelation(const lexer::LexerNode *currentLexedNode) {
            ParsingState state = removeWhiteSpacesAndComment(currentLexedNode);
            if (state.currentLexedNode != currentLexedNode) return {state.currentLexedNode, new ParserNode(Token::AnyParent)};
            return ParsingState{currentLexedNode, nullptr};
        }

        ParsingState tryParseSelectorsRelation(const lexer::LexerNode *currentLexedNode) {
            ParsingState state = tryParseDirectParentRelation(currentLexedNode);
            if (!state.currentParsedNode) state = tryParseAnyParentRelation(currentLexedNode);
            return state;
        }

        ParsingState tryParseSelectorsList(const lexer::LexerNode *currentLexedNodeStart, bool nested = false) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            ParsingState state;
            ParsingState selectorsRelationState;

            ParserNode *currentParsedNode = new ParserNode{Token::SelectorsList};

            if (nested) {
                if (currentLexedNode->token() == lexer::Token::Ampersand) {
                    currentParsedNode->addChild(new ParserNode{Token::SameElement});
                    currentLexedNode = currentLexedNode->next();
                }

                ParsingState state = tryParseDirectParentRelation(currentLexedNode);
                if (state.currentParsedNode) {
                    currentParsedNode->addChild(state.currentParsedNode);
                    currentLexedNode = state.currentLexedNode;
                }
            }

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
                    currentParsedNode->addChild(new ParserNode{Token::SameElement});
                    currentLexedNode = state.currentLexedNode;
                    currentParsedNode->addChild(state.currentParsedNode);
                    state = tryParseSelector(currentLexedNode);
                }

                // end of lexed list reached
                if (!currentLexedNode) break;

                selectorsRelationState = tryParseSelectorsRelation(currentLexedNode);
            } while (selectorsRelationState.currentLexedNode && selectorsRelationState.currentParsedNode);

            if (!currentParsedNode->nbChildren()) {
                delete currentParsedNode;
                return ParsingState{currentLexedNodeStart, nullptr, new std::list<ErrorMessage>({{ErrorType::LOG, "No selectors found"}})};
            }
            return ParsingState{currentLexedNode, currentParsedNode};
        }

        ParsingState tryParseSelectorsBlock(const lexer::LexerNode *currentLexedNodeStart, bool nested = false) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            ParserNode *currentParsedNodeRoot = new ParserNode{Token::SelectorsBlock};
            while (true) {
                ParsingState state = tryParseSelectorsList(currentLexedNode, nested);
                if (!state.currentParsedNode) {
                    delete currentParsedNodeRoot;
                    return ParsingState{currentLexedNodeStart, nullptr, state.errors};
                }
                if (!state.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
                currentParsedNodeRoot->addChild(state.currentParsedNode);
                removedSpacesState = removeSpaces(state.currentLexedNode);
                if (!removedSpacesState.currentLexedNode || removedSpacesState.currentLexedNode->token() != lexer::Token::Comma) {
                    return ParsingState{state.currentLexedNode, currentParsedNodeRoot};
                }
                currentLexedNode = removedSpacesState.currentLexedNode->next(); // skipping comma
            }
        }

        ParsingState tryParseRuleName(const lexer::LexerNode *currentLexedNode) {
            ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {lexer::Token::RawName})) return ParsingState{currentLexedNode, nullptr};
            return ParsingState{removedSpacesState.currentLexedNode->next(),
                                new ParserNode{Token::RuleName, removedSpacesState.currentLexedNode->value()}};
        }

        // try to parse a unit and if successful, add the given value as a child of the unit
        ParsingState tryParseUnit(const lexer::LexerNode *currentLexedNode, ParserNode *value) {
            if (currentLexedNode && currentLexedNode->token() == lexer::Token::Unit) {
                ParserNode *unit = new ParserNode{Token::Unit, currentLexedNode->value()};
                unit->addChild(value);
                value = unit;
                currentLexedNode = currentLexedNode->next();
            }
            return ParsingState{currentLexedNode, value};
        }

        ParsingState tryParseRuleValue(const lexer::LexerNode *currentLexedNode) {
            const lexer::LexerNode *currentLexedNodeStart = currentLexedNode;
            ParserNode *node;
            ParsingState state;
            ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};
            const lexer::LexerNode *value = removedSpacesState.currentLexedNode;

            switch (value->token()) {
            case lexer::Token::Int:
                return tryParseUnit(value->next(), new ParserNode{Token::Int, value->value()});
            case lexer::Token::Float:
                return tryParseUnit(value->next(), new ParserNode{Token::Float, value->value()});
            case lexer::Token::Bool:
                return ParsingState{value->next(), new ParserNode{Token::Bool, value->value()}};
            case lexer::Token::String:
                return ParsingState{value->next(), new ParserNode{Token::String, value->value()}};
            case lexer::Token::Sharp:
                if (isTokenIn(value->next(), {lexer::Token::RawName, lexer::Token::Int}) && isValidHex(value->next()->value())) {
                    return ParsingState{value->next()->next(), new ParserNode{Token::Hex, value->next()->value()}};
                }
                return ParsingState{currentLexedNodeStart, nullptr};
            case lexer::Token::RawName:
                return ParsingState{currentLexedNodeStart->next(), new ParserNode{Token::EnumValue, currentLexedNodeStart->value()}};
            case lexer::Token::OpeningParenthesis:
                node = new ParserNode{Token::Tuple};
                do {
                    currentLexedNode = removeWhiteSpaces(currentLexedNode->next()).currentLexedNode;
                    if (!currentLexedNode) break;

                    state = tryParseRuleValue(currentLexedNode);
                    currentLexedNode = state.currentLexedNode;
                    if (isAnyNull(state)) break;
                    node->addChild(state.currentParsedNode);
                    currentLexedNode = removeWhiteSpaces(currentLexedNode).currentLexedNode;
                } while (currentLexedNode && currentLexedNode->token() == lexer::Token::Comma);
                if (!currentLexedNode || currentLexedNode->token() != lexer::Token::ClosingParenthesis) {
                    delete node;
                    return ParsingState{currentLexedNodeStart, nullptr};
                }
                if (!node->nbChildren()) {
                    delete node;
                    return ParsingState{currentLexedNodeStart, nullptr, createErrorList(ErrorType::ERROR, "Empty tuple", currentLexedNodeStart)};
                }
                return ParsingState{currentLexedNode->next(), node};
            default:
                return ParsingState{currentLexedNodeStart, nullptr};
            }
        }

        ParsingState tryParseRuleAssignment(const lexer::LexerNode *currentLexedNodeStart) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            ParsingState state;
            ParserNode *assignment = new ParserNode{Token::Assignment};

            state = tryParseRuleName(currentLexedNode);
            if (isAnyNull(state)) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::LOG, "Not a rule assignment", currentLexedNode, state.errors)};
            }
            assignment->addChild(state.currentParsedNode);

            removedSpacesState = removeWhiteSpacesAndComment(state.currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {lexer::Token::Colon})) {
                delete assignment;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "Missing a colon in rule assignment", removedSpacesState.currentLexedNode)};
            }

            removedSpacesState = removeWhiteSpacesAndComment(removedSpacesState.currentLexedNode->next());
            state = tryParseRuleValue(removedSpacesState.currentLexedNode);
            if (isAnyNull(state)) {
                delete assignment;
                return ParsingState{
                    currentLexedNodeStart, nullptr,
                    createErrorList(ErrorType::ERROR, "Missing a rule value in rule assignment", removedSpacesState.currentLexedNode, state.errors)};
            }
            assignment->addChild(state.currentParsedNode);

            removedSpacesState = removeWhiteSpacesAndComment(state.currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {lexer::Token::SemiColon})) {
                delete assignment;
                return ParsingState{
                    currentLexedNodeStart, nullptr,
                    createErrorList(ErrorType::ERROR, "Missing a semi colon after rule assignment", removedSpacesState.currentLexedNode)};
            }
            return ParsingState{removedSpacesState.currentLexedNode->next(), assignment};
        }

        ParsingState tryParseSelectorsAndBlock(const lexer::LexerNode *currentLexedNodeStart, bool nested = false);

        ParsingState tryParseRulesBlock(const lexer::LexerNode *currentLexedNodeStart) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            ParsingState removedSpacesState;
            ParsingState state;
            std::list<ErrorMessage> *errors = nullptr;

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {lexer::Token::OpeningCurlyBracket}))
                return ParsingState{currentLexedNodeStart, nullptr};
            currentLexedNode = removedSpacesState.currentLexedNode->next();

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};
            currentLexedNode = removedSpacesState.currentLexedNode;

            ParserNode *currentParsedNodeRoot = new ParserNode{Token::BlockDeclarations};

            while (true) {
                state = tryParseRuleAssignment(currentLexedNode);
                errors = state.errors;
                if (!state.currentParsedNode) {
                    state = tryParseSelectorsAndBlock(currentLexedNode, true);
                }
                if (state.errors) errors = appendErrors(errors, state.errors);
                if (!state.currentParsedNode) break;
                currentParsedNodeRoot->addChild(state.currentParsedNode);
                currentLexedNode = state.currentLexedNode;
                removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
                if (removedSpacesState.currentLexedNode) currentLexedNode = removedSpacesState.currentLexedNode;
            }

            removedSpacesState = removeWhiteSpacesAndComment(currentLexedNode);
            if (!isTokenIn(removedSpacesState.currentLexedNode, {lexer::Token::ClosingCurlyBracket})) {
                delete currentParsedNodeRoot;

                if (errors) { // if there was errors during the parsing of a rule assignment, and the current token does not mean end of rule
                              // block, then it's a malformed text
                    return ParsingState{currentLexedNodeStart, nullptr, errors};
                }
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "Missing a closing curly bracket", removedSpacesState.currentLexedNode)};
            }
            return ParsingState{removedSpacesState.currentLexedNode->next(), currentParsedNodeRoot};
        }

        ParsingState tryParseImport(const lexer::LexerNode *currentLexedNodeStart) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            if (currentLexedNode->value() != "import") return ParsingState{currentLexedNodeStart, nullptr};
            ParsingState state = removeWhiteSpaces(currentLexedNode->next());
            if (state.currentLexedNode && state.currentLexedNode->token() == lexer::Token::String) {
                currentLexedNode = state.currentLexedNode->next();
                if (!currentLexedNode || currentLexedNode->token() != lexer::Token::SemiColon) {
                    return ParsingState{currentLexedNodeStart, nullptr,
                                        createErrorList(ErrorType::ERROR, "Missing a semi-colon after import", currentLexedNode)};
                }
                return ParsingState{currentLexedNode->next(), new ParserNode{Token::Import, state.currentLexedNode->value()}};
            }
            return ParsingState{currentLexedNodeStart, nullptr};
        }

        ParsingState tryParseAtRule(const lexer::LexerNode *currentLexedNodeStart) {
            std::list<ErrorMessage> *errors = nullptr;

            if (currentLexedNodeStart->token() != lexer::Token::At || currentLexedNodeStart->next()->token() != lexer::Token::RawName) {
                return ParsingState{currentLexedNodeStart, nullptr};
            }

            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart->next();

            ParsingState selectorParsingState = tryParseImport(currentLexedNode);
            errors = appendErrors(errors, selectorParsingState.errors);

            if (!selectorParsingState.currentParsedNode) return ParsingState{currentLexedNodeStart, nullptr, errors};
            return selectorParsingState;
        }

        ParsingState tryParseSelectorsAndBlock(const lexer::LexerNode *currentLexedNodeStart, bool nestedBlock) {
            ParsingState state;

            state = removeWhiteSpacesAndComment(currentLexedNodeStart);
            if (isNull(state)) return ParsingState{currentLexedNodeStart, nullptr};

            state = tryParseSelectorsBlock(state.currentLexedNode, nestedBlock);
            if (!state.currentLexedNode || !state.currentParsedNode)
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::LOG, "Not a valid selectors block", currentLexedNodeStart, state.errors)};

            ParserNode *currentParsedNodeRoot = new ParserNode{Token::StyleBlock};
            currentParsedNodeRoot->addChild(state.currentParsedNode);

            state = tryParseRulesBlock(state.currentLexedNode);
            if (!state.currentParsedNode) {
                delete currentParsedNodeRoot;
                return ParsingState{currentLexedNodeStart, nullptr,
                                    createErrorList(ErrorType::ERROR, "Invalid rules block", state.currentLexedNode, state.errors)};
            }

            currentParsedNodeRoot->addChild(state.currentParsedNode);

            return ParsingState{state.currentLexedNode, currentParsedNodeRoot};
        }

        ParsingState tryParseFile(const lexer::LexerNode *currentLexedNodeStart) {
            const lexer::LexerNode *currentLexedNode = currentLexedNodeStart;
            ParsingState state = removeWhiteSpacesAndComment(currentLexedNode);
            std::list<ErrorMessage> *errors = nullptr;

            ParserNode *firstParsedNode = nullptr;
            ParserNode *currentParsedNode = nullptr;

            while (state.currentLexedNode) {
                currentLexedNode = state.currentLexedNode;
                state = tryParseSelectorsAndBlock(currentLexedNode);
                if (!state.currentParsedNode) {
                    errors = state.errors;
                    state = tryParseAtRule(currentLexedNode);
                }
                if (!state.currentParsedNode) {
                    errors = appendErrors(errors, state.errors);
                    break;
                }
                if (!firstParsedNode) {
                    firstParsedNode = state.currentParsedNode;
                    currentParsedNode = firstParsedNode;
                }
                else currentParsedNode = currentParsedNode->next(state.currentParsedNode);
                state = removeWhiteSpacesAndComment(state.currentLexedNode);
            }

            return ParsingState{currentLexedNode, firstParsedNode, errors};
        }
    }

    ParseResult parse(lexer::LexerNode *currentNode, ParsingBlock block) {
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

        ParserNode *rootNode = new ParserNode(Token::NullRoot);
        rootNode->addChild(finalState.currentParsedNode);

#ifdef DEBUG
        std::clog << "Final parsed tree :\n";
        rootNode->debugDisplay(std::clog);
        std::clog << "\n";
#endif

        return {rootNode, errorMessages};
    }
} // namespace style
