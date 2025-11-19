#ifndef DESERIALIZATION_TESTS_HPP
#define DESERIALIZATION_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/style/style_deserializer.hpp"
#include "../test_config.hpp"

namespace deserializationTests {
    test::Result checkStyleComponentDataList(const style::StyleComponentDataList *testedData, const style::StyleComponentDataList *expectedData);
    test::Result checkStyleValue(style::StyleValue *testedValue, style::StyleValue *expectedValue);
    test::Result checkStyleRule(const style::StyleRule *testedRule, const style::StyleRule *expectedRule);
    test::Result checkStyleMap(const style::StyleValuesMap *testedStyleMap, const style::StyleValuesMap *expectedStyleMap);
    test::Result checkStyleBlocks(const std::list<style::StyleBlock *> *testedStyleBlocks, const std::list<style::StyleBlock *> *expectedStyleBlocks);
    test::Result testDeserializationFromFile(const std::string &fileName, const std::list<style::StyleBlock *> *expectedStyleBlocks);
    test::Result testDeserialization(const std::string &style, const std::list<style::StyleBlock *> *expectedStyleBlocks);

    /**
     * This method will catch exceptions who are not subclasses of std::exception but will not check if they are similar to T.
     */
    template <typename T>
    test::Result checkDeserializationError(const std::string &style);

    void testsDeserialization(test::Tests *tests);

} // namespace deserializationTests

#endif // DESERIALIZATION_TESTS_HPP
