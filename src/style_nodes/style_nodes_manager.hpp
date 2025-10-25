#ifndef STYLE_NODES_MANAGER_HPP
#define STYLE_NODES_MANAGER_HPP

#include "../style/style_component.hpp"
#include "../style/style_deserializer.hpp"
#include "style_node.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iterator>

namespace gui {
    namespace elementStyle {
        namespace manager {

            class StyleNodesManager {
                gui::elementStyle::StyleNode *elements = nullptr;
                std::list<style::StyleBlock *> style = {};
                int fileCount = 0;
                // file number: <file name, nb rules>
                std::unordered_map<int, std::pair<std::string, int>> files = {};
                std::string fontsPath = "";

                int findFile(const std::string &fileName);
                void updateRulesPrioritiesInElements(int oldFileNumber, int newwFileNumber, gui::elementStyle::StyleNode *element);
                void updateRulesPriorities(int fileNumber);
                void applySpecificStyleToElement(std::list<style::StyleBlock *> specificStyle, gui::elementStyle::StyleNode *elementStyle,
                                                 bool recursive = false);
                bool elementSelectorsCompatibles(gui::elementStyle::StyleNode *elementStyle, const style::StyleComponentDataList *componentsList);
                bool elementSelectorsCompatiblesLoop(style::StyleComponentDataList::const_reverse_iterator componentDataIt,
                                                     style::StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                                     gui::elementStyle::StyleNode *elementStyle);

            public:
                ~StyleNodesManager();
                void setParentElementStyle(gui::elementStyle::StyleNode *elementStyle) { elements = elementStyle; }
                void setFontsPath(const std::string &path);
                const std::string &getFontsPath() const;
                // returns a number who corresponds to the file number (used to delete it) or -1 if not added
                int addStyleFile(const std::string &fileName);
                // returns a number who corresponds to the style pseudo-file number (used to delete it) or -1 if not added
                int addStyle(const std::string &style);
                void removeStyleInElements(int fileNumber, gui::elementStyle::StyleNode *element);
                void removeStyle(int fileNumber);

                // TODO: Are both methods needed?
                void applyStyleToElement(gui::elementStyle::StyleNode *elementStyle, bool recursive = false);
                void addElementStyle(gui::elementStyle::StyleNode *elementStyle);
            };

        } // namespace manager
    } // namespace elementStyle
} // namespace gui

#endif // STYLE_NODES_MANAGER_HPP
