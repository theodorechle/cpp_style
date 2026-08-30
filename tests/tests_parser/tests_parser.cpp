#include "tests_parser.hpp"
#include "../../cpp_commons/src/text.hpp"
#include "../../src/abstract_configuration.hpp"
#include "../../src/lexer.hpp"
#include "../test_config.hpp"

namespace testsParser {

    test::Result testLexerAndParser(bool equal, const std::string &expression, const style::DeserializationNode *expected,
                                    style::parser::ParsingBlock block) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test lexing and parsing '\n" << expression << "'\n";
        try {
            style::DeserializationNode *tokens = style::Lexer().lexe(expression, config);
            style::parser::ParseResult result = style::parser::parse(tokens, block);
            if (areSameNodes(result.node, expected) == equal) testResult = test::Result::SUCCESS;
            else {
                if (equal) {
                    std::ostringstream firstStream;
                    std::ostringstream secondStream;
                    expected->debugDisplay(firstStream);
                    result.node->debugDisplay(secondStream);
                    commons::showDiff(firstStream.str(), secondStream.str(), "Expected", "Actual");
                }
                else std::cerr << "Expected different nodes\n";
                testResult = test::Result::FAILURE;
            }
            delete result.node;
            delete result.errors;
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

    test::Result testParserError(const std::string &expression, style::parser::ErrorMessage expectedError, style::parser::ParsingBlock block) {
        style::config::Config *config = testConfig();
        test::Result testResult = test::Result::FAILURE;
        std::cout
            << "Test if lexing and parsing\n'\n"
            << expression
            << "\n'\n gives the error \""
            << style::parser::errorMessageToString(expectedError)
            << " : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        style::parser::ParseResult result;
        tokens = style::Lexer().lexe(expression, config);
        result = style::parser::parse(tokens, block);

        std::cout << "Errors:\n";
        for (style::parser::ErrorMessage error : *result.errors) {
            std::cout << "- " << errorMessageToString(error) << "\n";
            if (error.type == expectedError.type && error.message == expectedError.message) {
                testResult = test::Result::SUCCESS;
            }
        }

        delete tokens;
        delete result.node;
        delete result.errors;
        delete config;
        std::cout << "\n";
        return testResult;
    }

} // namespace testsParser
