#include "config_tests/config_tests.hpp"
#include "deserialization_tests/deserialization_tests.hpp"
#include "tests_lexer/tests_lexer.hpp"
#include "tests_parser/tests_parser.hpp"

int main() {
    test::Tests tests = test::Tests();
    configTests::configTests(&tests);
    testsLexer::testsLexer(&tests);
    testsParser::testsParser(&tests);
    deserializationTests::testsDeserialization(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
