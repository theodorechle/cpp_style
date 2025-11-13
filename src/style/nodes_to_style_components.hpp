#ifndef NODES_TO_STYLE_COMPONENT_HPP
#define NODES_TO_STYLE_COMPONENT_HPP

#include "deserialization_node.hpp"
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
        DeserializationNode *tree = nullptr;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<StyleComponentDataList *> *> requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();
        std::list<StyleBlock *> *styleDefinitions = nullptr;

        DeserializationNode *importStyle(const std::string &fileName);

        static DeserializationNode *joinStyleDeclarations(DeserializationNode *firstDeclarations, DeserializationNode *secondDeclarations);
        static void moveNestedBlocksToRoot(DeserializationNode *style);
        void flattenStyle(DeserializationNode *style);
        void removeEmptyBlocks(DeserializationNode *style);

        std::list<StyleComponentDataList *> *convertStyleComponents();

        StyleValue *convertStyleNodeToStyleValue(DeserializationNode *node);

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
        std::list<StyleBlock *> *convert(DeserializationNode *tree, int fileNumber, int *ruleNumber);
    };

} // namespace style

#endif // NODES_TO_STYLE_COMPONENT_HPP
