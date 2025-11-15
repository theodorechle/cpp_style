#include "deserialization_tests/deserialization_tests.hpp"
#include "tests_parser/tests_parser.hpp"
#include "style_node_tests/style_node_tests.hpp"
#include "tests_lexer/tests_lexer.hpp"
#include "tests_parser/tests_parser.hpp"

int main() {
    test::Tests tests = test::Tests();
    testsLexer::testsLexer(&tests);
    testsParser::testsParser(&tests);
    deserializationTests::testsDeserialization(&tests);
    styleNodesTests::testsStyleNodes(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
