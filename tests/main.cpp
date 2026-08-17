#include "../cpp_tests/src/tests.hpp"
#include "config_tests/config_tests.hpp"
#include "deserialization_tests/deserialization_tests.hpp"
#include "tests_lexer/tests_lexer.hpp"
#include "tests_parser//tests_parser_selectors_block.hpp"
#include "tests_parser/tests_parser_file_block.hpp"

int main() {
    test::Tests tests = test::Tests(0, true);
    configTests::configTests(&tests);
    testsLexer::testsLexer(&tests);
    tests.beginTestBlock("Tests parser");
    testsParserFileBlock::testsParsingFileBlock(&tests);
    testsParserSelectorsBlock::testsParsingSelectorsBlock(&tests);
    tests.endTestBlock();
    deserializationTests::testsDeserialization(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
