#ifndef NODE_TO_STYLE_COMPONENT_LIST_HPP
#define NODE_TO_STYLE_COMPONENT_LIST_HPP

#include "node.hpp"
#include "style_component.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <list>
#include <tuple>

namespace style {

    class NodeToStyleComponentList {
        Node *tree;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<StyleComponentDataList *> *> requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();
        std::list<StyleBlock *> *styleDefinitions = nullptr;

        static Node *importStyle(const std::string &fileName);

        static Node *joinStyleDeclarations(Node *firstDeclarations, Node *secondDeclarations);
        static void moveNestedBlocksToRoot(Node *style);
        static void flattenStyle(Node *style);

        std::list<StyleComponentDataList *> *convertStyleComponents();

        StyleValue *convertStyleNodeToStyleValue(Node *node);

        StyleValuesMap *convertAppliedStyle(int fileNumber, int *ruleNumber);

        /**
         * Does not accept a null pointer for "components" parameter
         */
        std::list<StyleBlock *> *createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt,
                                                       StyleComponentDataList *components, StyleValuesMap *appliedStyle);

        int computeRuleSpecifity(StyleComponentDataList *ruleComponents);

        void convertStyleBlock(int fileNumber, int *ruleNumber);

    public:
        std::list<StyleBlock *> *convert(Node *tree, int fileNumber, int *ruleNumber);
    };

} // namespace style

#endif // NODE_TO_STYLE_COMPONENT_LIST_HPP
