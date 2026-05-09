#include "style_deserializer.hpp"
#include "deserialization_node.hpp"
#include "nodes_to_style_components.hpp"
#include <fstream>
#include <sstream>

namespace style {

    std::list<StyleDefinition *> *StyleDeserializer::deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber,
                                                                         const config::Config *config) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
        buffer << file.rdbuf();

        return deserialize(buffer.str(), fileNumber, ruleNumber, config);
    }

    std::list<StyleDefinition *> *StyleDeserializer::deserialize(const std::string &style, int fileNumber, int *ruleNumber,
                                                                 const config::Config *config) {

        std::list<StyleDefinition *> *deserializedStyle = nullptr;
        DeserializationNode *tokens = nullptr;
        DeserializationNode *result = nullptr;
        deserializedStyle = NodesToStyleComponents(config).convert(style, fileNumber, ruleNumber);

        delete tokens;
        delete result;
        return deserializedStyle;
    }

} // namespace style
