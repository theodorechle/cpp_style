#include "tests_parser.hpp"
#include "../../src/abstract_configuration.hpp"
#include "../../src/lexer.hpp"
#include "../test_config.hpp"

namespace testsParser {

    test::Result testLexerAndParser(bool equal, const std::string &expression, const style::DeserializationNode *expected,
                                    style::parser::ParsingBlock block) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expression << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->debugDisplay(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *tokens = style::Lexer().lexe(expression, config);
            style::parser::ParseResult result = style::parser::parse(tokens, block);
            if (areSameNodes(result.node, expected) == equal) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
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
            << expectedError.message
            << "\" ("
            << style::parser::errorTypeToString(expectedError.type)
            << ") : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        style::parser::ParseResult result;
        tokens = style::Lexer().lexe(expression, config);
        result = style::parser::parse(tokens, block);

        for (style::parser::ErrorMessage error : *result.errors) {
            if (error.type == expectedError.type && error.message == expectedError.message) {
                testResult = test::Result::SUCCESS;
                break;
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
