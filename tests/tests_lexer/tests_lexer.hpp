#ifndef TESTS_LEXER_HPP
#define TESTS_LEXER_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/style/lexer.hpp"

namespace testsLexer {
    test::Result testLexer(bool equal, const std::string &expression, const style::DeserializationNode *expected);

    template <typename T>
    test::Result testLexerException(const std::string &expression);

    void testsLexer(test::Tests *tests);

} // namespace testsLexer

#endif // TESTS_LEXER_HPP