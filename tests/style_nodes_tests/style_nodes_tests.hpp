#ifndef STYLE_NODES_TESTS_HPP
#define STYLE_NODES_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/style/style_component.hpp"
#include "../../src/style_nodes/style_node.hpp"

namespace styleNodesTests {
    void displayValue(style::StyleValue *value);
    bool testValue(bool equal, style::StyleValue *value, style::StyleValue *expected, bool disaplyDebug = false);
    bool testRule(bool equal, const style::StyleRule &rule, const style::StyleRule &expected);

    void testsStyleNodes(test::Tests *tests);
} // namespace styleNodesTests

#endif // STYLE_NODES_TESTS_HPP