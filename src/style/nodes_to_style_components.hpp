#ifndef NODES_TO_STYLE_COMPONENT_HPP
#define NODES_TO_STYLE_COMPONENT_HPP

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

    class NodesToStyleComponents {
        const Config *config;
        Node *tree = nullptr;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<StyleComponentDataList *> *> requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();
        std::list<StyleBlock *> *styleDefinitions = nullptr;

        Node *importStyle(const std::string &fileName);

        static Node *joinStyleDeclarations(Node *firstDeclarations, Node *secondDeclarations);
        static void moveNestedBlocksToRoot(Node *style);
        void flattenStyle(Node *style);
        void removeEmptyBlocks(Node *style);

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
            NodesToStyleComponents(const Config *config): config{config} {}
        std::list<StyleBlock *> *convert(Node *tree, int fileNumber, int *ruleNumber);
    };

} // namespace style

#endif // NODES_TO_STYLE_COMPONENT_HPP
