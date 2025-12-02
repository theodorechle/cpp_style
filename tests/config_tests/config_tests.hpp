#ifndef CONFIG_TESTS_HPP
#define CONFIG_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/abstract_configuration.hpp"

namespace configTests {
    template <typename T>
    test::Result checkConfigError(const style::config::Config *config);
    test::Result checkConfigValid(const style::config::Config *config);

    void configTests(test::Tests *tests);
} // namespace configTests

#endif // CONFIG_TESTS_HPP
