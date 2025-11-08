#include "style_deserialization_tests/style_deserialization_tests.hpp"
#include "style_tests_lexer_and_parser/style_tests_lexer_and_parser.hpp"
#include "style_nodes_tests/style_nodes_tests.hpp"

int main() {
    test::Tests tests = test::Tests();
    styleTestsLexerAndParser::testsLexerAndParser(&tests);
    styleDeserializationTests::testsDeserialization(&tests);
    styleNodesTests::testsStyleNodes(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
