#ifndef TESTS_LEXER_HPP
#define TESTS_LEXER_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/lexer_node.hpp"

namespace testsLexer {
    test::Result testLexer(bool equal, const std::string &expression, const style::lexer::LexerNode *expected);

    template <typename T>
    test::Result testLexerException(const std::string &expression);

    void testsLexer(test::Tests *tests);

} // namespace testsLexer

#endif // TESTS_LEXER_HPP
