#include "tests_parser.hpp"
namespace testsParser {

    test::Result testLexerAndParser(bool equal, const std::string &expression, const style::DeserializationNode *expected, style::ParsingBlock block) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expression << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->debugDisplay(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *tokens = style::Lexer().lexe(expression, config);
            style::DeserializationNode *result = style::Parser().parse(tokens, block);
            if (areSameNodes(result, expected) == equal) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
            delete tokens;
            delete config;
        }
        catch (const std::exception &e) {
            testResult = test::Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        return testResult;
    }

    std::string getFileContent(std::string fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
} // namespace testsParser
