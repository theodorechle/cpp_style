#include "nodes_to_style_components.hpp"
#include "style_component.hpp"

namespace style {

    StyleComponentType tokenTypeToStyleComponentType(Token token) {
        switch (token) {
        case Token::StarWildcard:
            return StyleComponentType::StarWildcard;
        case Token::ElementName:
            return StyleComponentType::ElementName;
        case Token::Class:
            return StyleComponentType::Class;
        case Token::Modifier:
            return StyleComponentType::Modifier;
        case Token::Identifier:
            return StyleComponentType::Identifier;
        default:
            return StyleComponentType::Null;
        }
    }

    StyleValueType tokenTypeToStyleValueType(Token token) {
        switch (token) {
        case Token::Int:
            return StyleValueType::Int;
        case Token::Float:
            return StyleValueType::Float;
        case Token::Bool:
            return StyleValueType::Bool;
        case Token::String:
            return StyleValueType::String;
        case Token::Tuple:
            return StyleValueType::Tuple;
        case Token::Function:
            return StyleValueType::Function;
        case Token::Unit:
            return StyleValueType::Unit;
        case Token::Hex:
            return StyleValueType::Hex;
        case Token::EnumValue:
            return StyleValueType::EnumValue;
        default:
            return StyleValueType::Null;
        }
    }

    StyleRelation tokenTypeToStyleRelation(Token token) {
        switch (token) {
        case Token::DirectParent:
            return StyleRelation::DirectParent;
        case Token::AnyParent:
            return StyleRelation::AnyParent;
        case Token::SameElement:
            return StyleRelation::SameElement;
        default:
            return StyleRelation::Null;
        }
    }

    Node *NodesToStyleComponents::importStyle(const std::string &fileName) {
        Node *tokens = nullptr;
        Node *result = nullptr;
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
        buffer << file.rdbuf();
        try {
            tokens = Lexer(config, buffer.str()).getResult();
            result = Parser(tokens).getFinalTree();
        }
        catch (const ParserException &) {
            delete tokens;
            delete result;
            throw;
        }
        catch (const LexerException &) {
            delete tokens;
            delete result;
            throw;
        }
        delete tokens;
        return result;
    }

    Node *NodesToStyleComponents::joinStyleDeclarations(Node *firstDeclarations, Node *secondDeclarations) {
        Node *newDeclarations = new Node(Token::NullRoot);
        Node *actualDeclaration;
        Node *secondDeclarationsIt;

        while (firstDeclarations != nullptr) {
            secondDeclarationsIt = secondDeclarations;
            while (secondDeclarationsIt != nullptr) {
                actualDeclaration = newDeclarations->appendNext(firstDeclarations->copyNodeWithChilds());
                if (tokenTypeToStyleRelation(secondDeclarationsIt->child()->token()) == StyleRelation::Null)
                    actualDeclaration->addChild(new Node(Token::AnyParent));
                if (tokenTypeToStyleRelation(secondDeclarationsIt->child()->token()) == StyleRelation::SameElement)
                    secondDeclarationsIt->deleteSpecificChild(secondDeclarationsIt->child());
                actualDeclaration->addChild(secondDeclarationsIt->child()->copyNodeWithChildsAndNexts());
                secondDeclarationsIt = secondDeclarationsIt->next();
            }
            firstDeclarations = firstDeclarations->next();
        }

        Node *root = newDeclarations;
        newDeclarations = newDeclarations->next();
        root->next(nullptr);
        delete root;
        return newDeclarations;
    }

    void NodesToStyleComponents::moveNestedBlocksToRoot(Node *style) {
        Node *blockDeclarations = style->child();
        Node *definition = blockDeclarations->next()->child();
        Node *nextDeclaration;

        while (definition != nullptr) {
            if (definition->token() == Token::StyleBlock) {
                definition->child()->replaceChild(definition->child()->child(),
                                                     joinStyleDeclarations(blockDeclarations->child(), definition->child()->child()));
                nextDeclaration = definition->next();
                definition->parent()->removeSpecificChild(definition);
                definition->next(style->next());
                style->next(definition);
                definition->setParent(style->parent());
                style = definition;
                definition = nextDeclaration;
                continue;
            }
            definition = definition->next();
        }
    }

    void NodesToStyleComponents::flattenStyle(Node *style) {
        if (style == nullptr) return;
        if (style->token() == Token::NullRoot) style = style->child();
        while (style != nullptr) {
            if (style->token() == Token::StyleBlock) moveNestedBlocksToRoot(style);
            else if (style->token() == Token::Import) {
                Node *importedStyle = importStyle(style->value());
                if (importedStyle != nullptr) {
                    importedStyle->addChild(style->next());
                    style->next(importedStyle->child());
                    importedStyle->setChild(nullptr);
                    delete importedStyle;
                }
            }
            style = style->next();
        }
    }

    std::list<StyleComponentDataList *> *NodesToStyleComponents::convertStyleComponents() {
        std::list<StyleComponentDataList *> *styleComponentsLists;
        StyleComponentDataList *requiredStyleComponents;
        Node *declaration;
        Node *componentDeclaration;
        Token nextDeclarationToken;
        StyleComponentType styleComponentType;
        StyleRelation styleRelationToken;
        std::string currentValue;

        if (tree == nullptr || tree->token() != Token::BlockDeclaration) return nullptr;

        styleComponentsLists = new std::list<StyleComponentDataList *>();
        declaration = tree->child();
        // loop through declarations
        while (declaration != nullptr) {
            if (declaration->token() != Token::Declaration) { // invalid block declaration
                delete styleComponentsLists;
                return nullptr;
            }
            requiredStyleComponents = new StyleComponentDataList();
            componentDeclaration = declaration->child();
            // loop through each value in a declaration
            while (componentDeclaration != nullptr) {
                styleComponentType = tokenTypeToStyleComponentType(componentDeclaration->token());
                currentValue = componentDeclaration->value();
                componentDeclaration = componentDeclaration->next();

                // if invalid declaration, skip it
                if (styleComponentType == StyleComponentType::Null) continue;

                if (componentDeclaration == nullptr) styleRelationToken = StyleRelation::SameElement; // last value
                else {
                    nextDeclarationToken = componentDeclaration->token();
                    if (tokenTypeToStyleComponentType(nextDeclarationToken) != StyleComponentType::Null) {
                        styleRelationToken = StyleRelation::SameElement;
                    }
                    else {
                        styleRelationToken = tokenTypeToStyleRelation(nextDeclarationToken);
                        if (styleRelationToken != StyleRelation::Null) componentDeclaration = componentDeclaration->next();
                    }
                }
                if (styleRelationToken != StyleRelation::Null) {
                    requiredStyleComponents->push_back(std::pair(StyleComponentData(currentValue, styleComponentType), styleRelationToken));
                }
            }
            styleComponentsLists->push_back(requiredStyleComponents);
            declaration = declaration->next();
        }
        return styleComponentsLists;
    }

    StyleValue *NodesToStyleComponents::convertStyleNodeToStyleValue(Node *node) {
        if (node == nullptr) return nullptr;
        StyleValueType type;
        Node *next;
        StyleValue *styleValue;
        StyleValue *tmpStyleValue;
        StyleValue *styleNext;

        type = tokenTypeToStyleValueType(node->token());
        if (type == StyleValueType::Null) return nullptr;

        styleValue = new StyleValue();
        styleValue->setValue(node->value());
        styleValue->setType(type);

        styleValue->setChild(convertStyleNodeToStyleValue(node->child()));

        next = node->next();
        tmpStyleValue = styleValue;
        while (next != nullptr) {
            styleNext = convertStyleNodeToStyleValue(next);
            if (styleNext == nullptr) break;
            tmpStyleValue->setNext(styleNext);
            tmpStyleValue = styleNext;
            next = next->next();
        }
        return styleValue;
    }

    StyleValuesMap *NodesToStyleComponents::convertAppliedStyle(int fileNumber, int *ruleNumber) {
        StyleValuesMap *appliedStyleMap;
        StyleValue *styleValue;
        std::string ruleName;
        Node *definition;
        Node *oldTree;
        Node *ruleNameNode;
        Token token;
        if (tree == nullptr || tree->token() != Token::BlockDefinition) return nullptr;

        appliedStyleMap = new StyleValuesMap();
        definition = tree->child();
        while (definition != nullptr) {
            token = definition->token();
            if (token == Token::Assignment) {
                ruleNameNode = definition->child();
                if (ruleNameNode == nullptr || ruleNameNode->token() != Token::RuleName) {
                    definition = definition->next();
                    continue;
                }
                ruleName = ruleNameNode->value();
                ruleNameNode = ruleNameNode->next();
                if (!isNodeNull(ruleNameNode)) {
                    styleValue = convertStyleNodeToStyleValue(ruleNameNode);
                    if (styleValue != nullptr) {
                        (*appliedStyleMap)[ruleName] = StyleRule(styleValue->copy(), true, 0, fileNumber, *ruleNumber);
                        (*ruleNumber)++;
                    }
                }
            }
            else if (token == Token::StyleBlock) {
                oldTree = tree;
                tree = definition;
                convertStyleBlock(fileNumber, ruleNumber);
                tree = oldTree;
            }
            // if other token type, just don't use the definition
            definition = definition->next();
        }

        return appliedStyleMap;
    }

    std::list<StyleBlock *> *
    NodesToStyleComponents::createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt,
                                                    StyleComponentDataList *components, StyleValuesMap *appliedStyleMap) {

        if (components == nullptr) return nullptr;
        std::list<StyleBlock *> *styleComponentList = new std::list<StyleBlock *>();
        std::list<StyleBlock *> *tmpStyleComponentList;
        StyleComponentDataList::const_iterator componentsIt;
        bool islistHead = std::next(componentsListIt) == requiredStyleComponentsLists.cend();
        if (islistHead) { // if at end of the declaration list
            for (StyleComponentDataList *componentsDataList : **componentsListIt) {
                componentsIt = std::prev(components->end());
                std::copy(componentsDataList->begin(), componentsDataList->end(), std::back_inserter(*components));
                int specificity = computeRuleSpecifity(components);
                for (std::pair<const std::string, StyleRule> &rule : *appliedStyleMap) {
                    rule.second.specificity = specificity;
                }
                styleComponentList->push_back(new StyleBlock(*components, *appliedStyleMap));
                components->erase(componentsIt, components->end());
            }
        }
        else {
            for (StyleComponentDataList *componentsList : **componentsListIt) {
                componentsIt = std::prev(components->end());
                std::copy(componentsList->begin(), componentsList->end(), std::back_inserter(*components));
                tmpStyleComponentList = createStyleComponents(std::next(componentsListIt), components, appliedStyleMap);
                if (tmpStyleComponentList != nullptr) {
                    styleComponentList->splice(styleComponentList->end(), *tmpStyleComponentList);
                    delete tmpStyleComponentList;
                }
                components->erase(componentsIt, components->end());
            }
        }
        return styleComponentList;
    }

    int NodesToStyleComponents::computeRuleSpecifity(StyleComponentDataList *ruleComponents) {
        int specificity = 0;
        for (std::pair<StyleComponentData, StyleRelation> component : *ruleComponents) {
            switch (component.first.second) {
            case StyleComponentType::Identifier:
                specificity += 100;
                break;
            case StyleComponentType::Modifier:
            case StyleComponentType::Class:
                specificity += 10;
                break;
            case StyleComponentType::ElementName:
                specificity += 1;
                break;
            default:
                break;
            }
        }
        return specificity;
    }

    void NodesToStyleComponents::convertStyleBlock(int fileNumber, int *ruleNumber) {
        std::list<StyleComponentDataList *> *styleComponentsLists;
        if (tree == nullptr || tree->token() != Token::StyleBlock) return;
        tree = tree->child();
        styleComponentsLists = convertStyleComponents();
        if (styleComponentsLists == nullptr || styleComponentsLists->empty()) {
            delete styleComponentsLists;
            return;
        }

        tree = tree->next();
        StyleValuesMap *appliedStyleMap = convertAppliedStyle(fileNumber, ruleNumber);
        tree = tree->parent();
        if (appliedStyleMap == nullptr || appliedStyleMap->empty()) {
            delete styleComponentsLists;
            delete appliedStyleMap;
            return;
        }

        requiredStyleComponentsLists.push_back(styleComponentsLists);
        StyleComponentDataList components = StyleComponentDataList();
        std::list<StyleBlock *> *finalStyleComponents = createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyleMap);
        delete appliedStyleMap;
        for (StyleComponentDataList *componentDataList : *(requiredStyleComponentsLists.back())) {
            delete componentDataList;
        }
        delete requiredStyleComponentsLists.back();
        requiredStyleComponentsLists.pop_back();
        if (finalStyleComponents != nullptr) {
            styleDefinitions->splice(styleDefinitions->end(), *finalStyleComponents);
            delete finalStyleComponents;
        }
    }

    std::list<StyleBlock *> *NodesToStyleComponents::convert(Node *styleTree, int fileNumber, int *ruleNumber) {
        *ruleNumber = 0;
        if (styleTree->token() != Token::NullRoot) return nullptr;
        styleDefinitions = new std::list<StyleBlock *>();

        flattenStyle(styleTree);
#ifdef DEBUG
        std::cerr << "flattened style\n";
        styleTree->display(std::cerr);
#endif
        tree = styleTree->child();

        while (tree != nullptr) {
            convertStyleBlock(fileNumber, ruleNumber);
            tree = tree->next();
        }

        requiredStyleComponentsLists.clear();

        return styleDefinitions;
    }

} // namespace style
