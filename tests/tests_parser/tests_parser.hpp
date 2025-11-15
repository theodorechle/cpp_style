#ifndef TESTS_LEXER_AND_PARSER_HPP
#define TESTS_LEXER_AND_PARSER_HPP

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

namespace testsParser {
    const std::string TESTS_FILES_DIR = "tests/tests_parser/tests-files";

    std::string getFileContent(std::string fileName);

    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::DeserializationNode *expected);

    template <typename T>
    test::Result testLexerAndParserException(const std::string &expression);

    void testsParser(test::Tests *tests);
} // namespace testsParser

#endif // TESTS_LEXER_AND_PARSER_HPP