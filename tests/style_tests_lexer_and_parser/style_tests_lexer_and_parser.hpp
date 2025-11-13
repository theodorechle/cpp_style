#ifndef STYLE_TESTS_LEXER_AND_PARSER_HPP
#define STYLE_TESTS_LEXER_AND_PARSER_HPP

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/style/lexer.hpp"
#include "../../src/style/deserialization_node.hpp"
#include "../../src/style/parser.hpp"
#include "../style_config.hpp"

namespace styleTestsLexerAndParser {
    const std::string TESTS_FILES_DIR = "tests/style_tests_lexer_and_parser/tests-files";
    test::Result testLexer(bool equal, const std::string &expr, const style::DeserializationNode *expected);
    test::Result testParser(bool equal, style::DeserializationNode *expr, const style::DeserializationNode *expected);
    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::DeserializationNode *expected);

    template <typename T>
    test::Result testLexerException(const std::string &expression);

    template <typename T>
    test::Result testLexerAndParserException(const std::string &expression);
    std::string getFileContent(std::string fileName);

    void testsLexerAndParser(test::Tests *tests);
} // namespace styleTestsLexerAndParser

#endif // STYLE_TESTS_LEXER_AND_PARSER_HPP