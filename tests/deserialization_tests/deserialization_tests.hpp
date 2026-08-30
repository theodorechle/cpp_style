#ifndef DESERIALIZATION_TESTS_HPP
#define DESERIALIZATION_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/style_component.hpp"

namespace deserializationTests {
    test::Result checkStyleComponentDataList(const style::SelectorDataList *testedData, const style::SelectorDataList *expectedData);
    test::Result checkStyleValue(style::StyleValue *testedValue, style::StyleValue *expectedValue);
    test::Result checkStyleRule(const style::StyleRule *testedRule, const style::StyleRule *expectedRule);
    test::Result checkStyleMap(const style::RulesMap *testedStyleMap, const style::RulesMap *expectedStyleMap);
    test::Result checkStyleDefinitions(const std::list<style::StyleDefinition *> *testedStyleDefinitions, const std::list<style::StyleDefinition *> *expectedStyleDefinitions);
    test::Result testDeserializationFromFile(const std::string &fileName, const std::list<style::StyleDefinition *> *expectedStyleDefinitions);
    test::Result testDeserialization(const std::string &style, const std::list<style::StyleDefinition *> *expectedStyleDefinitions);
    void testsDeserialization(test::Tests *tests);

} // namespace deserializationTests

#endif // DESERIALIZATION_TESTS_HPP
