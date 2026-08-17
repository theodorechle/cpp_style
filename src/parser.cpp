#include "parser.hpp"
#include "deserialization_node.hpp"
#include "tokens.hpp"
#include <regex>

namespace style {

    bool Parser::isValidName(const std::string &str, size_t start, size_t end) {
        size_t i;
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

    DeserializationNode *Parser::parse(DeserializationNode *currentNode, ParsingBlock block) {
        ParsingState finalState;
        switch (block) {
        case ParsingBlock::FILE:
            finalState = tryParseFile(currentNode);
            break;
        case ParsingBlock::SELECTORS:
            finalState = tryParseSelectorsBlock(currentNode);
            break;
        }

        DeserializationNode *rootNode = new DeserializationNode(Token::NullRoot);
        rootNode->addChild(finalState.currentParsedNode);

#ifdef DEBUG
        std::clog << "Final parsed tree :\n";
        rootNode->debugDisplay(std::clog);
        std::clog << "\n";
#endif

        return rootNode;
    }

    bool Parser::isTokenIn(const DeserializationNode *node, std::vector<Token> tokens) {
        if (!node) return false;
        for (Token token : tokens) {
            if (node->token() != token) return false;
        }
        return true;
    }

    bool Parser::isNull(const ParsingState &parsingState) { return !parsingState.currentLexedNode && !parsingState.currentParsedNode; }

    ParsingState Parser::removeSpaces(const DeserializationNode *currentLexedNode) {
        const DeserializationNode *startNode = currentLexedNode;
        const DeserializationNode *currentNode = startNode;
        while (isTokenIn(currentNode, {Token::Space})) {
            currentNode = currentNode->next();
        }
        if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
        return ParsingState{currentNode, nullptr};
    }

    ParsingState Parser::removeWhiteSpaces(const DeserializationNode *currentLexedNode) {
        const DeserializationNode *startNode = currentLexedNode;
        const DeserializationNode *currentNode = startNode;
        while (isTokenIn(currentNode, {Token::Space, Token::LineBreak})) {
            currentNode = currentNode->next();
        }
        if (startNode == currentNode) return ParsingState{currentLexedNode, nullptr};
        return ParsingState{currentNode, nullptr};
    }

    ParsingState Parser::tryParseElementName(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::RawName && isValidElementOrRuleName(currentLexedNode->value())) {
            return ParsingState{currentLexedNode->next(), new DeserializationNode(Token::ElementName, currentLexedNode->value())};
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseClass(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::Dot) {
            const DeserializationNode *nextNode = currentLexedNode->next();
            if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                return ParsingState{nextNode->next(), new DeserializationNode(Token::Class, nextNode->value())};
            }
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseIdentifier(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::Sharp) {
            const DeserializationNode *nextNode = currentLexedNode->next();
            if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                return ParsingState{nextNode->next(), new DeserializationNode(Token::Identifier, nextNode->value())};
            }
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseModifier(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::Colon) {
            const DeserializationNode *nextNode = currentLexedNode->next();
            if (isTokenIn(nextNode, {Token::RawName}) && isValidElementOrRuleName(nextNode->value())) {
                return ParsingState{nextNode->next(), new DeserializationNode(Token::Modifier, nextNode->value())};
            }
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseSelector(const DeserializationNode *currentLexedNode) {
        ParsingState selectorParsingState = tryParseElementName(currentLexedNode);
        if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseClass(currentLexedNode);
        if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseIdentifier(currentLexedNode);
        if (!selectorParsingState.currentParsedNode) selectorParsingState = tryParseModifier(currentLexedNode);
        return selectorParsingState;
    }

    ParsingState Parser::tryParseDirectParentRelation(const DeserializationNode *currentLexedNode) {
        ParsingState state = removeSpaces(currentLexedNode);
        if (state.currentLexedNode->token() != Token::GreaterThan) return ParsingState{};
        state.currentLexedNode = state.currentLexedNode->next();
        state.currentParsedNode = new DeserializationNode(Token::DirectParent);
        ParsingState stateAfterFinalSpacesRemoval = removeSpaces(state.currentLexedNode);
        if (stateAfterFinalSpacesRemoval.currentLexedNode != state.currentLexedNode) {
            state.currentLexedNode = stateAfterFinalSpacesRemoval.currentLexedNode;
        }
        return state;
    }

    ParsingState Parser::tryParseAnyParentRelation(const DeserializationNode *currentLexedNode) {
        ParsingState state = removeSpaces(currentLexedNode);
        if (state.currentLexedNode != currentLexedNode) return {state.currentLexedNode, new DeserializationNode(Token::AnyParent)};
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseSelectorsRelation(const DeserializationNode *currentLexedNode) {
        ParsingState state = tryParseDirectParentRelation(currentLexedNode);
        if (!state.currentParsedNode) state = tryParseAnyParentRelation(currentLexedNode);
        return state;
    }

    ParsingState Parser::tryParseSelectorsList(const DeserializationNode *currentLexedNodeStart) {
        const DeserializationNode *currentLexedNode = currentLexedNodeStart;
        ParsingState state;
        ParsingState selectorsRelationState;

        state = removeSpaces(currentLexedNode);
        if (!state.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};
        currentLexedNode = state.currentLexedNode;

        DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::SelectorsList};
        DeserializationNode *currentParsedNode = currentParsedNodeRoot;

        do {
            // parse selectors with no separator between
            state = tryParseSelector(selectorsRelationState.currentParsedNode ? selectorsRelationState.currentLexedNode : currentLexedNode);
            if (!state.currentParsedNode) return ParsingState{currentLexedNode, currentParsedNode};
            if (selectorsRelationState.currentParsedNode) currentParsedNode->addChild(selectorsRelationState.currentParsedNode);
            if (!state.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};

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

        if (!currentParsedNode) {
            delete currentParsedNodeRoot;
            return ParsingState{currentLexedNodeStart, nullptr};
        }
        return ParsingState{currentLexedNode, currentParsedNodeRoot};
    }

    ParsingState Parser::tryParseSelectorsBlock(const DeserializationNode *currentLexedNodeStart) {
        const DeserializationNode *currentLexedNode = currentLexedNodeStart;
        ParsingState removedSpacesState;
        DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::SelectorsBlock};
        while (true) {
            removedSpacesState = removeSpaces(currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            ParsingState state = tryParseSelectorsList(removedSpacesState.currentLexedNode);
            if (!state.currentLexedNode) {
                delete currentParsedNodeRoot;
                return ParsingState{currentLexedNodeStart, nullptr};
            }
            if (!state.currentParsedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            currentLexedNode = state.currentLexedNode;
            currentParsedNodeRoot->addChild(state.currentParsedNode);
            removedSpacesState = removeSpaces(state.currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            currentLexedNode = removedSpacesState.currentLexedNode;
            if (currentLexedNode->token() != Token::Comma) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            currentLexedNode = currentLexedNode->next(); // skipping comma
        }
    }

    ParsingState Parser::tryParseRuleName(const DeserializationNode *currentLexedNode) {
        ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode);
        if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::RawName})) return ParsingState{currentLexedNode, nullptr};
        return ParsingState{currentLexedNode->next(), new DeserializationNode{Token::RuleName, currentLexedNode->value()}};
    }

    ParsingState Parser::tryParseRuleValue(const DeserializationNode *currentLexedNode) {
        ParsingState removedSpacesState = removeWhiteSpaces(currentLexedNode->next());
        if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNode, nullptr};
        const DeserializationNode *value = removedSpacesState.currentLexedNode;

        switch (value->token()) {
        case Token::Int:
            return ParsingState{value->next(), new DeserializationNode{Token::Int, value->value()}};
        case Token::Float:
            return ParsingState{value->next(), new DeserializationNode{Token::Float, value->value()}};
        case Token::Bool:
            return ParsingState{value->next(), new DeserializationNode{Token::Bool, value->value()}};
        case Token::Sharp:
            if (value->next() && value->next()->token() == Token::RawName) {
                return ParsingState{value->next()->next(), new DeserializationNode{Token::Hex, value->next()->value()}};
            }
            return ParsingState{currentLexedNode, nullptr};
        default:
            return ParsingState{currentLexedNode, nullptr};
        }
    }

    ParsingState Parser::tryParseRuleAssignment(const DeserializationNode *currentLexedNodeStart) {
        const DeserializationNode *currentLexedNode = currentLexedNodeStart;
        ParsingState removedSpacesState;
        ParsingState state;
        DeserializationNode *assignment = new DeserializationNode{Token::Assignment};

        state = tryParseRuleName(currentLexedNode);
        if (isNull(state)) {
            delete assignment;
            return ParsingState{currentLexedNodeStart, nullptr};
        }
        assignment->addChild(state.currentParsedNode);

        removedSpacesState = removeWhiteSpaces(state.currentLexedNode);
        if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::Colon})) {
            delete assignment;
            return ParsingState{currentLexedNodeStart, nullptr};
        }

        state = tryParseRuleValue(removedSpacesState.currentLexedNode->next());
        if (isNull(state)) {
            delete assignment;
            return ParsingState{currentLexedNodeStart, nullptr};
        }
        assignment->addChild(state.currentParsedNode);

        if (!isTokenIn(state.currentLexedNode, {Token::SemiColon})) {
            delete assignment;
            return ParsingState{currentLexedNodeStart, nullptr};
        }
        return ParsingState{state.currentLexedNode->next(), assignment};
    }

    ParsingState Parser::tryParseRulesBlock(const DeserializationNode *currentLexedNodeStart) {
        const DeserializationNode *currentLexedNode = currentLexedNodeStart;
        ParsingState removedSpacesState;
        ParsingState state;

        removedSpacesState = removeSpaces(currentLexedNode);
        if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::OpeningCurlyBracket})) return ParsingState{currentLexedNodeStart, nullptr};
        currentLexedNode = removedSpacesState.currentLexedNode->next();

        DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::BlockDeclarations};
        removedSpacesState = removeSpaces(currentLexedNode);
        if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNodeStart, nullptr};

        while (true) {
            state = tryParseRuleAssignment(currentLexedNode);
            if (!state.currentParsedNode) break;
            currentParsedNodeRoot->addChild(state.currentParsedNode);
            currentLexedNode = state.currentLexedNode;
            removedSpacesState = removeSpaces(currentLexedNode);
            if (removedSpacesState.currentLexedNode) currentLexedNode = removedSpacesState.currentLexedNode;
        }

        removedSpacesState = removeWhiteSpaces(currentLexedNode);
        if (!isTokenIn(removedSpacesState.currentLexedNode, {Token::ClosingCurlyBracket})) {
            delete currentParsedNodeRoot;
            return ParsingState{currentLexedNodeStart, nullptr};
        }
        return ParsingState{removedSpacesState.currentLexedNode->next(), currentParsedNodeRoot};
    }

    ParsingState Parser::tryParseImport(const DeserializationNode *currentLexedNode) { return ParsingState{currentLexedNode, nullptr}; }

    ParsingState Parser::tryParseAtRule(const DeserializationNode *currentLexedNode) { return ParsingState{currentLexedNode, nullptr}; }

    ParsingState Parser::tryParseFile(const DeserializationNode *currentLexedNodeStart) {
        ParsingState state;
        state = removeWhiteSpaces(currentLexedNodeStart);
        if (isNull(state)) return ParsingState{currentLexedNodeStart, nullptr};

        state = tryParseSelectorsBlock(state.currentLexedNode);
        if (!state.currentLexedNode || !state.currentParsedNode) return ParsingState{currentLexedNodeStart, nullptr};

        DeserializationNode *currentParsedNodeRoot = new DeserializationNode{Token::StyleBlock};
        currentParsedNodeRoot->addChild(state.currentParsedNode);

        state = tryParseRulesBlock(state.currentLexedNode);
        if (!state.currentParsedNode) {
            delete currentParsedNodeRoot;
            return ParsingState{currentLexedNodeStart, nullptr};
        }

        currentParsedNodeRoot->addChild(state.currentParsedNode);

        return ParsingState{state.currentLexedNode, currentParsedNodeRoot};
    }
} // namespace style
