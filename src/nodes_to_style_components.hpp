#ifndef NODES_TO_STYLE_COMPONENT_HPP
#define NODES_TO_STYLE_COMPONENT_HPP

#include "abstract_configuration.hpp"
#include "deserialization_node.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "style_component.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <tuple>

namespace style {

    class NodesToStyleComponents {
        const config::Config *_config = nullptr;
        DeserializationNode *tree = nullptr;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<StyleComponentDataList *> *> requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();
        std::list<StyleDefinition *> *styleDefinitions = nullptr;

        DeserializationNode *importStyle(const std::string &fileName);

        static DeserializationNode *joinStyleDeclarations(DeserializationNode *firstDeclarations, DeserializationNode *secondDeclarations);
        static void moveNestedBlocksToRoot(DeserializationNode *style);
        void flattenStyle(DeserializationNode *style);

        bool ruleNodesValid(const DeserializationNode *ruleNode, const config::ConfigRuleNode *configNode);
        bool ruleValid(const DeserializationNode *rule);
        void filterRulesWithConfiguration(DeserializationNode *style);

        std::list<StyleComponentDataList *> *convertStyleComponents();

        StyleValue *convertStyleNodeToStyleValue(DeserializationNode *node);

        StyleValuesMap *convertAppliedStyle(int fileNumber, int *ruleNumber);

        /**
         * Does not accept a null pointer for "components" parameter
         */
        std::list<StyleDefinition *> *createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt,
                                                       StyleComponentDataList *components, StyleValuesMap *appliedStyle);

        int computeRuleSpecifity(StyleComponentDataList *ruleComponents);

        void convertStyleDefinition(int fileNumber, int *ruleNumber);

    public:
        NodesToStyleComponents(const config::Config *config) : _config{config} {}
        std::list<StyleDefinition *> *convert(DeserializationNode *tree, int fileNumber, int *ruleNumber);
    };

} // namespace style

#endif // NODES_TO_STYLE_COMPONENT_HPP
