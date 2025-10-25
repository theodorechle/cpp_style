#include "style_deserializer.hpp"

namespace style {

    std::list<StyleBlock *> *StyleDeserializer::deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
        buffer << file.rdbuf();

        return deserialize(buffer.str(), fileNumber, ruleNumber);
    }

    std::list<StyleBlock *> *StyleDeserializer::deserialize(const std::string &style, int fileNumber, int *ruleNumber) {
        std::list<StyleBlock *> *deserializedStyle = nullptr;
        Node *tokens = nullptr;
        Node *result = nullptr;
        try {
            tokens = Lexer(style).getResult();
            result = Parser(tokens).getFinalTree();
            deserializedStyle = NodeToStyleComponentList().convert(result, fileNumber, ruleNumber);
        }
        catch (const ParserException &) {
            delete tokens;
            delete result;
            throw;
        }
        catch (const LexerException &) {
            delete tokens;
            delete result;
            throw;
        }
        delete tokens;
        delete result;
        return deserializedStyle;
    }

} // namespace Style
