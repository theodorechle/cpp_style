#ifndef NODES_TO_STYLE_COMPONENT_HPP
#define NODES_TO_STYLE_COMPONENT_HPP

#include "abstract_configuration.hpp"
#include "deserialization_node.hpp"
#include "style_component.hpp"

#include <list>
#include <string>

namespace style {

    class NodesToStyleComponents {
        const config::Config *_config = nullptr;
        DeserializationNode *tree = nullptr;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<SelectorDataList *> *> requiredStyleComponentsLists = std::list<std::list<SelectorDataList *> *>();
        std::list<StyleDefinition *> *styleDefinitions = nullptr;

        DeserializationNode *deserializeStyle(const std::string &style);

        DeserializationNode *deserializeStyleFromFile(const std::string &fileName);

        static DeserializationNode *joinStyleDeclarations(DeserializationNode *firstDeclarations, DeserializationNode *secondDeclarations);
        static void moveNestedBlocksToRoot(DeserializationNode *style);
        void flattenStyle(DeserializationNode *style);

        bool ruleNodesValid(const DeserializationNode *ruleNode, const config::ConfigRuleNode *configNode);
        bool ruleValid(const DeserializationNode *rule);
        void filterRulesWithConfiguration(DeserializationNode *style);

        std::list<SelectorDataList *> *convertStyleSelectors();

        StyleValue *convertStyleNodeToStyleValue(DeserializationNode *node);

        RulesMap *convertAppliedStyle(int fileNumber, int *ruleNumber);

        /**
         * Does not accept a null pointer for "components" parameter
         */
        std::list<StyleDefinition *> *createStyleComponents(std::list<std::list<SelectorDataList *> *>::const_iterator componentsListIt,
                                                            SelectorDataList *components, RulesMap *appliedStyle);

        int computeRuleSpecifity(SelectorDataList *ruleComponents);

        void convertStyleDefinition(int fileNumber, int *ruleNumber);

    public:
        NodesToStyleComponents(const config::Config *config) : _config{config} {}
        std::list<StyleDefinition *> *convert(const std::string &style, int fileNumber, int *ruleNumber);

        SelectorDataList *convertSelectors(const std::string &selectors);
    };

} // namespace style

#endif // NODES_TO_STYLE_COMPONENT_HPP
