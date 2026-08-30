#include "nodes_to_style_components.hpp"
#include "deserialization_node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "style_component.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

namespace style {

    SelectorType tokenTypeToSelectorType(Token token) {
        switch (token) {
        case Token::StarWildcard:
            return SelectorType::StarWildcard;
        case Token::ElementName:
            return SelectorType::ElementName;
        case Token::Class:
            return SelectorType::Class;
        case Token::Modifier:
            return SelectorType::Modifier;
        case Token::Identifier:
            return SelectorType::Identifier;
        default:
            return SelectorType::Null;
        }
    }

    ValueType tokenTypeToValueType(Token token) {
        switch (token) {
        case Token::Int:
            return ValueType::Int;
        case Token::Float:
            return ValueType::Float;
        case Token::Bool:
            return ValueType::Bool;
        case Token::String:
            return ValueType::String;
        case Token::Tuple:
            return ValueType::Tuple;
        case Token::Function:
            return ValueType::Function;
        case Token::Unit:
            return ValueType::Unit;
        case Token::Hex:
            return ValueType::Hex;
        case Token::EnumValue:
            return ValueType::EnumValue;
        default:
            return ValueType::Null;
        }
    }

    SelectorsRelation tokenTypeToSelectorsRelation(Token token) {
        switch (token) {
        case Token::DirectParent:
            return SelectorsRelation::DirectParent;
        case Token::AnyParent:
            return SelectorsRelation::AnyParent;
        case Token::SameElement:
            return SelectorsRelation::SameElement;
        default:
            return SelectorsRelation::Null;
        }
    }

    DeserializationNode *NodesToStyleComponents::deserializeStyle(const std::string &style) {
        DeserializationNode *tokens = nullptr;
        parser::ParseResult result;
        config::configChecker(_config);
        tokens = Lexer().lexe(style, _config);
        result = parser::parse(tokens);
        delete tokens;

        for (parser::ErrorMessage error : *result.errors) {
            std::cerr << "Parsing Error: " << parser::errorTypeToString(error.type) << ": " << error.message << "\n";
        }
        delete result.errors;
        return result.node;
    }

    DeserializationNode *NodesToStyleComponents::deserializeStyleFromFile(const std::string &fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
        buffer << file.rdbuf();
        return deserializeStyle(buffer.str());
    }

    DeserializationNode *NodesToStyleComponents::joinStyleDeclarations(DeserializationNode *firstDeclarations,
                                                                       DeserializationNode *secondDeclarations) {
        DeserializationNode *newDeclarations = new DeserializationNode(Token::NullRoot);
        DeserializationNode *actualDeclaration;

        while (firstDeclarations != nullptr) {
            while (secondDeclarations != nullptr) {
                actualDeclaration = newDeclarations->appendNext(firstDeclarations->copyNodeWithChildren());
                if (tokenTypeToSelectorsRelation(secondDeclarations->child()->token()) == SelectorsRelation::Null)
                    actualDeclaration->addChild(new DeserializationNode(Token::AnyParent));
                if (tokenTypeToSelectorsRelation(secondDeclarations->child()->token()) == SelectorsRelation::SameElement)
                    secondDeclarations->deleteSpecificChild(secondDeclarations->child());
                actualDeclaration->addChild(secondDeclarations->child()->copyNodeWithChildrenAndNexts());
                secondDeclarations = secondDeclarations->next();
            }
            firstDeclarations = firstDeclarations->next();
        }

        DeserializationNode *root = newDeclarations;
        newDeclarations = newDeclarations->next();
        root->next(nullptr);
        delete root;
        return newDeclarations;
    }

    void NodesToStyleComponents::moveNestedBlocksToRoot(DeserializationNode *style) {
        DeserializationNode *blockDeclarations = style->child();
        DeserializationNode *content = blockDeclarations->next()->child();
        DeserializationNode *nextDeclaration;

        while (content != nullptr) {
            if (content->token() == Token::StyleBlock) {
                content->child()->replaceChild(content->child()->child(),
                                               joinStyleDeclarations(blockDeclarations->child(), content->child()->child()));
                nextDeclaration = content->next();
                content->parent()->removeSpecificChild(content);
                content->next(style->next());
                style->next(content);
                content->setParent(style->parent());
                style = content;
                content = nextDeclaration;
                continue;
            }
            content = content->next();
        }
    }

    void NodesToStyleComponents::flattenStyle(DeserializationNode *style) {
        if (style == nullptr) return;
        style = style->child();
        while (style != nullptr) {
            if (style->token() == Token::StyleBlock) moveNestedBlocksToRoot(style);
            else if (style->token() == Token::Import) {
                DeserializationNode *importedStyle = deserializeStyleFromFile(style->value());
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

    bool NodesToStyleComponents::ruleNodesValid(const DeserializationNode *ruleNode, const config::ConfigRuleNode *configNode) {
        if (ruleNode == nullptr && configNode == nullptr) return true;
        if (ruleNode == nullptr || configNode == nullptr) return false;
        if (ruleNode->token() != configNode->token()) return false;
        if (configNode->token() == Token::EnumValue) {
            const std::set<std::string> &allowedValues = static_cast<const config::ConfigRuleNodeEnum *>(configNode)->allowedValues();
            return allowedValues.find(ruleNode->value()) != allowedValues.cend();
        }
        return true;
    }

    bool NodesToStyleComponents::ruleValid(const DeserializationNode *rule) {
        const DeserializationNode *ruleName = rule->child();
        const DeserializationNode *ruleValue = ruleName->next();
        std::unordered_map<std::string, std::vector<const config::ConfigRuleNode *>>::const_iterator configRules =
            _config->rules.find(ruleName->value());
        if (configRules == _config->rules.cend()) return false;
        for (const config::ConfigRuleNode *configRule : configRules->second) {
            if (ruleNodesValid(ruleValue, configRule)) return true;
        }
        return false;
    }

    void NodesToStyleComponents::filterRulesWithConfiguration(DeserializationNode *style) {
        if (style == nullptr) return;
        DeserializationNode *block = style->child();
        DeserializationNode *declaration;
        DeserializationNode *rule;
        DeserializationNode *nextRule;
        DeserializationNode *next;

        while (block) {
            declaration = block->getLastChild();
            if (!declaration) {
                block = block->next();
                continue;
            }
            rule = declaration->child();
            while (rule) {
                nextRule = rule->next();
                if (rule->token() == Token::Assignment && !ruleValid(rule)) {
#ifdef DEBUG
                    std::cerr << "invalid rule:\n";
                    rule->debugDisplay();
                    std::unordered_map<std::string, std::vector<const config::ConfigRuleNode *>>::const_iterator configRules =
                        _config->rules.find(rule->child()->value());
                    if (configRules != _config->rules.cend()) {
                        std::cerr << "available config rules:\n";
                        for (const config::ConfigRuleNode *configRule : configRules->second) {
                            configRule->debugDisplay();
                        }
                    }

#endif
                    declaration->removeSpecificChild(rule);
                    delete rule;
                }
                rule = nextRule;
            }
            if (declaration->nbChildren() == 0) {
                next = block->next();
                style->removeSpecificChild(block);
                delete block;
                block = next;
            }
            else block = block->next();
        }
    }

    std::list<SelectorDataList *> *NodesToStyleComponents::convertStyleSelectors() {
        std::list<SelectorDataList *> *selectorsLists;
        SelectorDataList *requiredSelectors;
        DeserializationNode *declaration;
        DeserializationNode *declarationPart;
        SelectorType selectorType;
        SelectorsRelation styleRelationToken;
        std::string currentValue;

        if (tree == nullptr || tree->token() != Token::SelectorsBlock) return nullptr;

        selectorsLists = new std::list<SelectorDataList *>();
        declaration = tree->child();
        // loop through declarations
        while (declaration != nullptr) {
            requiredSelectors = new SelectorDataList();
            declarationPart = declaration->child();
            // loop through each value in a declaration
            while (declarationPart != nullptr) {
                selectorType = tokenTypeToSelectorType(declarationPart->token());
                currentValue = declarationPart->value();
                declarationPart = declarationPart->next();

                if (declarationPart == nullptr) styleRelationToken = SelectorsRelation::SameElement; // last value
                else {
                    styleRelationToken = tokenTypeToSelectorsRelation(declarationPart->token());
                    if (styleRelationToken != SelectorsRelation::Null) declarationPart = declarationPart->next();
                }
                if (styleRelationToken != SelectorsRelation::Null) {
                    requiredSelectors->push_back(std::pair(SelectorData(currentValue, selectorType), styleRelationToken));
                }
            }
            selectorsLists->push_back(requiredSelectors);
            declaration = declaration->next();
        }
        return selectorsLists;
    }

    StyleValue *NodesToStyleComponents::convertStyleNodeToStyleValue(DeserializationNode *node) {
        if (node == nullptr) return nullptr;
        ValueType type;
        StyleValue *styleValue;
        StyleValue *styleNext;

        type = tokenTypeToValueType(node->token());
        if (type == ValueType::Null) return nullptr;

        styleValue = new StyleValue(node->value(), type);

        styleValue->addChild(convertStyleNodeToStyleValue(node->child()));

        styleNext = convertStyleNodeToStyleValue(node->next());
        if (styleNext != nullptr) styleValue->next(styleNext);
        return styleValue;
    }

    RulesMap *NodesToStyleComponents::convertAppliedStyle(int fileNumber, int *ruleNumber) {
        RulesMap *appliedStyleMap;
        StyleValue *styleValue;
        std::string ruleName;
        DeserializationNode *rule;
        DeserializationNode *oldTree;
        DeserializationNode *ruleNameNode;
        Token token;
        if (tree == nullptr || tree->token() != Token::BlockDeclarations) return nullptr;

        appliedStyleMap = new RulesMap();
        rule = tree->child();
        while (rule != nullptr) {
            token = rule->token();
            if (token == Token::Assignment) {
                ruleNameNode = rule->child();
                if (ruleNameNode == nullptr || ruleNameNode->token() != Token::RuleName) {
                    rule = rule->next();
                    continue;
                }
                ruleName = ruleNameNode->value();
                ruleNameNode = ruleNameNode->next();
                if (!isNodeNull(ruleNameNode)) {
                    styleValue = convertStyleNodeToStyleValue(ruleNameNode);
                    if (styleValue != nullptr) {
                        (*appliedStyleMap).insert_or_assign(ruleName, StyleRule(styleValue, true, 0, fileNumber, *ruleNumber));
                        delete styleValue;
                        (*ruleNumber)++;
                    }
                }
            }
            else if (token == Token::StyleBlock) {
                oldTree = tree;
                tree = rule;
                convertStyleDefinition(fileNumber, ruleNumber);
                tree = oldTree;
            }
            // if other token type, just don't use it
            rule = rule->next();
        }

        return appliedStyleMap;
    }

    std::list<StyleDefinition *> *
    NodesToStyleComponents::createStyleComponents(std::list<std::list<SelectorDataList *> *>::const_iterator componentsListIt,
                                                  SelectorDataList *components, RulesMap *appliedStyleMap) {
        if (components == nullptr) return nullptr;
        std::list<StyleDefinition *> *styleComponentList = new std::list<StyleDefinition *>();
        std::list<StyleDefinition *> *tmpStyleComponentList;
        SelectorDataList::const_iterator componentsIt;
        if (std::next(componentsListIt) == requiredStyleComponentsLists.cend()) { // if at end of the declaration list
            for (SelectorDataList *componentsDataList : **componentsListIt) {
                componentsIt = std::prev(components->end());
                // appliedStyleMap[0]->second.value._value is modified on the next line
                // is it because the memory was freed (but still used because of an invalid pointer) and this line is when this memory is reused
                // elsewhere? or a memory overflow?
                std::copy(componentsDataList->begin(), componentsDataList->end(), std::back_inserter(*components));
                int specificity = computeRuleSpecifity(components);
                for (std::pair<const std::string, StyleRule> &rule : *appliedStyleMap) {
                    rule.second.specificity = specificity;
                }
                styleComponentList->push_back(new StyleDefinition(*components, *appliedStyleMap));
                if (componentsIt == components->end()) components->clear();
                else components->erase(componentsIt, components->end());
            }
        }
        else {
            for (SelectorDataList *componentsList : **componentsListIt) {
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

    int NodesToStyleComponents::computeRuleSpecifity(SelectorDataList *ruleSelectors) {
        int specificity = 0;
        for (std::pair<SelectorData, SelectorsRelation> selector : *ruleSelectors) {
            switch (selector.first.second) {
            case SelectorType::Identifier:
                specificity += 100;
                break;
            case SelectorType::Modifier:
            case SelectorType::Class:
                specificity += 10;
                break;
            case SelectorType::ElementName:
                specificity += 1;
                break;
            default:
                break;
            }
        }
        return specificity;
    }

    void NodesToStyleComponents::convertStyleDefinition(int fileNumber, int *ruleNumber) {
        std::list<SelectorDataList *> *styleComponentsLists;
        if (tree == nullptr || tree->token() != Token::StyleBlock) return;
        tree = tree->child();

        styleComponentsLists = convertStyleSelectors();
        if (styleComponentsLists == nullptr || styleComponentsLists->empty()) {
            delete styleComponentsLists;
            return;
        }

        tree = tree->next();
        RulesMap *appliedStyleMap = convertAppliedStyle(fileNumber, ruleNumber);
        tree = tree->parent();
        if (appliedStyleMap == nullptr || appliedStyleMap->empty()) {
            delete styleComponentsLists;
            delete appliedStyleMap;
            return;
        }

        requiredStyleComponentsLists.push_back(styleComponentsLists); // TODO: I think there is one useless list
        SelectorDataList components = SelectorDataList();
        std::list<StyleDefinition *> *finalStyleComponents =
            createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyleMap);

        delete appliedStyleMap;
        for (SelectorDataList *componentDataList : *(requiredStyleComponentsLists.back())) {
            delete componentDataList;
        }
        delete requiredStyleComponentsLists.back();
        requiredStyleComponentsLists.pop_back();
        if (finalStyleComponents != nullptr) {
            styleDefinitions->splice(styleDefinitions->end(), *finalStyleComponents);
            delete finalStyleComponents;
        }
    }

    std::list<StyleDefinition *> *NodesToStyleComponents::convert(const std::string &style, int fileNumber, int *ruleNumber) {
        *ruleNumber = 0;

        DeserializationNode *styleTree = deserializeStyle(style);

        flattenStyle(styleTree);
#ifdef DEBUG
        std::clog << "flattened style\n";
        styleTree->debugDisplay(std::clog);
#endif
        filterRulesWithConfiguration(styleTree);
#ifdef DEBUG
        std::clog << "filtered style\n";
        styleTree->debugDisplay(std::clog);
#endif
        tree = styleTree->child();

        styleDefinitions = new std::list<StyleDefinition *>();

        while (tree != nullptr) {
            convertStyleDefinition(fileNumber, ruleNumber);
            tree = tree->next();
        }

        requiredStyleComponentsLists.clear();

        delete styleTree;

        return styleDefinitions;
    }

    SelectorDataList *NodesToStyleComponents::convertSelectors(const std::string &selectors) {
        DeserializationNode *nodes = deserializeStyle(selectors);
        delete nodes;
        return new SelectorDataList();
    }

} // namespace style
