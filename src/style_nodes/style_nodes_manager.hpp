#ifndef STYLE_NODES_MANAGER_HPP
#define STYLE_NODES_MANAGER_HPP

#include "../style/abstract_configuration.hpp"
#include "../style/style_component.hpp"
#include "../style/style_deserializer.hpp"
#include "style_node.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>

namespace style {
    namespace elementStyle {
        namespace manager {

            class StyleNodesManager {
                const Config *config;
                elementStyle::StyleNode *elements = nullptr;
                std::list<StyleBlock *> style = {};
                int fileCount = 0;
                // file number: <file name, nb rules>
                std::unordered_map<int, std::pair<std::string, int>> files = {};
                std::string fontsPath = "";

                int findFile(const std::string &fileName);
                void updateRulesPrioritiesInElements(int oldFileNumber, int newwFileNumber, elementStyle::StyleNode *element);
                void updateRulesPriorities(int fileNumber);
                void applySpecificStyleToElement(std::list<StyleBlock *> specificStyle, elementStyle::StyleNode *elementStyle,
                                                 bool recursive = false);
                bool elementSelectorsCompatibles(elementStyle::StyleNode *elementStyle, const StyleComponentDataList *componentsList);
                bool elementSelectorsCompatiblesLoop(StyleComponentDataList::const_reverse_iterator componentDataIt,
                                                     StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                                     elementStyle::StyleNode *elementStyle);

            public:
                StyleNodesManager(const Config *config) : config{config} {}
                ~StyleNodesManager();
                void setParentElementStyle(elementStyle::StyleNode *elementStyle) { elements = elementStyle; }
                void setFontsPath(const std::string &path);
                const std::string &getFontsPath() const;
                // returns a number who corresponds to the file number (used to delete it) or -1 if not added
                int addStyleFile(const std::string &fileName);
                // returns a number who corresponds to the style pseudo-file number (used to delete it) or -1 if not added
                int addStyle(const std::string &style);
                void removeStyleInElements(int fileNumber, elementStyle::StyleNode *element);
                void removeStyle(int fileNumber);

                // TODO: Are both methods needed?
                void applyStyleToElement(elementStyle::StyleNode *elementStyle, bool recursive = false);
                void addElementStyle(elementStyle::StyleNode *elementStyle);
            };

        } // namespace manager
    } // namespace elementStyle
} // namespace style

#endif // STYLE_NODES_MANAGER_HPP
