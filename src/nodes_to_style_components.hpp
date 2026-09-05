#ifndef NODES_TO_STYLE_COMPONENT_HPP
#define NODES_TO_STYLE_COMPONENT_HPP

#include "abstract_configuration.hpp"
#include "parser_node.hpp"
#include "style_component.hpp"

#include <list>
#include <string>

namespace style {

    class NodesToStyleComponents {
        const config::Config *_config = nullptr;
        parser::ParserNode *tree = nullptr;
        // for each inner style block, multiple components list definitions (separated by commas in the style files)
        std::list<std::list<SelectorDataList *> *> requiredStyleComponentsLists = std::list<std::list<SelectorDataList *> *>();
        std::list<StyleDefinition *> *styleDefinitions = nullptr;

        parser::ParserNode *deserializeStyle(const std::string &style);

        parser::ParserNode *deserializeStyleFromFile(const std::string &fileName);

        static parser::ParserNode *joinStyleDeclarations(parser::ParserNode *firstDeclarations, parser::ParserNode *secondDeclarations);
        static void moveNestedBlocksToRoot(parser::ParserNode *style);
        void flattenStyle(parser::ParserNode *style);

        bool ruleNodesValid(const parser::ParserNode *ruleNode, const config::ConfigRuleNode *configNode);
        bool ruleValid(const parser::ParserNode *rule);
        void filterRulesWithConfiguration(parser::ParserNode *style);

        std::list<SelectorDataList *> *convertStyleSelectors();

        StyleValue *convertStyleNodeToStyleValue(parser::ParserNode *node);

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
