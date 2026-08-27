#ifndef TESTS_LEXER_AND_PARSER_HPP
#define TESTS_LEXER_AND_PARSER_HPP

#include <bits/stdc++.h>
#include <string>

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/deserialization_node.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"
#include "../test_config.hpp"

namespace testsParser {
    const std::string TESTS_FILES_DIR = "tests/tests_parser/tests-files";

    std::string getFileContent(std::string fileName);

    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::DeserializationNode *expected,
                                    style::parser::ParsingBlock block = style::parser::ParsingBlock::FILE);

    /**
     * This method will try to find a matching ErrorMessage in the errors of the parser
     */
    test::Result testParserError(const std::string &style, style::parser::ErrorMessage expectedError,
                                     style::parser::ParsingBlock block = style::parser::ParsingBlock::FILE);

    template <typename T>
    test::Result testLexerException(const std::string &expression) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test if lexing \n'\n" << expression << "\n'\n raises an exception : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        style::DeserializationNode *result = nullptr;
        try {
            tokens = style::Lexer().lexe(expression, config);
            testResult = test::Result::FAILURE;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) testResult = test::Result::SUCCESS;
            else {
                testResult = test::Result::ERROR;
                std::cerr << "Error : " << exception.what();
            }
        }
        delete tokens;
        delete result;
        delete config;
        std::cout << "\n";
        return testResult;
    }

} // namespace testsParser

#endif // TESTS_LEXER_AND_PARSER_HPP
