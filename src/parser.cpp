#include "parser.hpp"
#include "deserialization_node.hpp"
#include "tokens.hpp"

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

    ParsingState Parser::emptyParsingState(const DeserializationNode *currentLexedNode) { return ParsingState{currentLexedNode, nullptr}; }

    ParsingState Parser::removeSpaces(const DeserializationNode *currentLexedNode) {
        const DeserializationNode *startNode = currentLexedNode;
        const DeserializationNode *currentNode = startNode;
        while (currentNode && currentNode->token() == Token::Space) {
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
            if (nextNode && nextNode->token() == Token::RawName && isValidElementOrRuleName(nextNode->value())) {
                return ParsingState{nextNode->next(), new DeserializationNode(Token::Class, nextNode->value())};
            }
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseIdentifier(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::Sharp) {
            const DeserializationNode *nextNode = currentLexedNode->next();
            if (nextNode && nextNode->token() == Token::RawName && isValidElementOrRuleName(nextNode->value())) {
                return ParsingState{nextNode->next(), new DeserializationNode(Token::Identifier, nextNode->value())};
            }
        }
        return ParsingState{currentLexedNode, nullptr};
    }

    ParsingState Parser::tryParseModifier(const DeserializationNode *currentLexedNode) {
        if (currentLexedNode->token() == Token::Colon) {
            const DeserializationNode *nextNode = currentLexedNode->next();
            if (nextNode && nextNode->token() == Token::RawName && isValidElementOrRuleName(nextNode->value())) {
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
        return ParsingState{nullptr, nullptr};
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
        if (!state.currentLexedNode) return ParsingState{nullptr, nullptr};
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
        DeserializationNode *currentParsedNode = currentParsedNodeRoot;
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
            currentParsedNode->addChild(state.currentParsedNode);
            removedSpacesState = removeSpaces(state.currentLexedNode);
            if (!removedSpacesState.currentLexedNode) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            currentLexedNode = removedSpacesState.currentLexedNode;
            if (currentLexedNode->token() != Token::Comma) return ParsingState{currentLexedNode, currentParsedNodeRoot};
            currentLexedNode = currentLexedNode->next(); // skipping comma
        }
    }

    ParsingState Parser::tryParseRuleAssignment(const DeserializationNode *currentLexedNode) { return ParsingState{nullptr, nullptr}; }

    ParsingState Parser::tryParseRulesBlock(const DeserializationNode *currentLexedNode) { return ParsingState{nullptr, nullptr}; }

    ParsingState Parser::tryParseImport(const DeserializationNode *currentLexedNode) { return ParsingState{nullptr, nullptr}; }

    ParsingState Parser::tryParseAtRule(const DeserializationNode *currentLexedNode) { return ParsingState{nullptr, nullptr}; }

    ParsingState Parser::tryParseFile(const DeserializationNode *currentLexedNode) { return ParsingState{nullptr, nullptr}; }
} // namespace style
