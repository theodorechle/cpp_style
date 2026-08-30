#include "deserialization_tests.hpp"
#include "../../../cpp_style/src/style_deserializer.hpp"
#include "../test_config.hpp"

namespace deserializationTests {

    test::Result checkStyleComponentDataList(const style::SelectorDataList *testedData, const style::SelectorDataList *expectedData) {
        if (testedData == nullptr && expectedData == nullptr) return test::Result::SUCCESS;
        if ((testedData == nullptr && expectedData != nullptr)
            || (testedData != nullptr && expectedData == nullptr)
            || (testedData->size() != expectedData->size())) {
            std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
            return test::Result::FAILURE;
        }
        style::SelectorDataList::const_iterator testedDataListIt = testedData->cbegin();
        style::SelectorDataList::const_iterator expectedDataListIt = expectedData->cbegin();
        while (testedDataListIt != testedData->cend()) {
            if (testedDataListIt->second != expectedDataListIt->second) {
                std::cerr
                    << selectorsRelationToString(testedDataListIt->second)
                    << " instead of relation "
                    << selectorsRelationToString(expectedDataListIt->second)
                    << "\n";
                return test::Result::FAILURE;
            }
            if (testedDataListIt->first.first != expectedDataListIt->first.first) {
                std::cerr << testedDataListIt->first.first << " instead of name " << expectedDataListIt->first.first << "\n";
                return test::Result::FAILURE;
            }
            if (testedDataListIt->first.second != expectedDataListIt->first.second) {
                std::cerr
                    << selectorTypeToString(testedDataListIt->first.second)
                    << " instead of type "
                    << selectorTypeToString(expectedDataListIt->first.second)
                    << "\n";
                return test::Result::FAILURE;
            }
            testedDataListIt++;
            expectedDataListIt++;
        }
        return test::Result::SUCCESS;
    }

    test::Result checkStyleValue(style::StyleValue *testedValue, style::StyleValue *expectedValue) {
        test::Result result;

        if (testedValue == nullptr && expectedValue == nullptr) return test::Result::SUCCESS;
        if ((testedValue == nullptr && expectedValue != nullptr) || (testedValue != nullptr && expectedValue == nullptr)) {
            std::cerr << "One of the rule is null\n";
            return test::Result::FAILURE;
        }
        if (testedValue->value() != expectedValue->value()) {
            std::cerr << "The name is different (have '" << testedValue->value() << "', expected '" << expectedValue->value() << "')\n";
            return test::Result::FAILURE;
        }
        if (testedValue->type() != expectedValue->type()) {
            std::cerr
                << "The type is different (have '"
                << valueTypeToString(testedValue->type())
                << "', expected '"
                << valueTypeToString(expectedValue->type())
                << "')\n";
            return test::Result::FAILURE;
        }

        result = checkStyleValue(testedValue->child(), expectedValue->child());
        if (result != test::Result::SUCCESS) return result;

        result = checkStyleValue(testedValue->next(), expectedValue->next());
        if (result != test::Result::SUCCESS) return result;
        return test::Result::SUCCESS;
    }

    test::Result checkStyleRule(const style::StyleRule *testedRule, const style::StyleRule *expectedRule) {
        if (testedRule == nullptr && expectedRule == nullptr) return test::Result::SUCCESS;
        if ((testedRule == nullptr && expectedRule != nullptr) || (testedRule != nullptr && expectedRule == nullptr)) {
            std::cerr << "One of the rule is null\n";
            return test::Result::FAILURE;
        }
        if (testedRule->fileNumber != expectedRule->fileNumber) {
            std::cerr << "The file number is different (have '" << testedRule->fileNumber << "', expected '" << expectedRule->fileNumber << "')\n";
            return test::Result::FAILURE;
        }
        if (testedRule->ruleNumber != expectedRule->ruleNumber) {
            std::cerr << "The rule number is different (have '" << testedRule->ruleNumber << "', expected '" << expectedRule->ruleNumber << "')\n";
            return test::Result::FAILURE;
        }
        if (testedRule->specificity != expectedRule->specificity) {
            std::cerr << "The specificity is different (have '" << testedRule->specificity << "', expected '" << expectedRule->specificity << "')\n";
            return test::Result::FAILURE;
        }
        if (testedRule->enabled != expectedRule->enabled) {
            std::cerr << "The enabled status is different (have '" << testedRule->enabled << "', expected '" << expectedRule->enabled << "')\n";
            return test::Result::FAILURE;
        }
        return checkStyleValue(testedRule->value, expectedRule->value);
    }

    test::Result checkStyleMap(const style::RulesMap *testedStyleMap, const style::RulesMap *expectedStyleMap) {
        style::RulesMap::const_iterator ruleIt;
        test::Result styleRuleCheckResult;
        if (testedStyleMap == nullptr && expectedStyleMap == nullptr) return test::Result::SUCCESS;
        if ((testedStyleMap == nullptr && expectedStyleMap != nullptr)
            || (testedStyleMap != nullptr && expectedStyleMap == nullptr)
            || (testedStyleMap->size() != expectedStyleMap->size())) {
            std::cerr << "One of the style maps list is null or the maps doesn't have the same size\n";
            return test::Result::FAILURE;
        }

        for (const std::pair<std::string, style::StyleRule> rule : *expectedStyleMap) {
            ruleIt = testedStyleMap->find(rule.first);
            if (ruleIt == testedStyleMap->cend()) {
                std::cerr << "Rule " << rule.first << " can't be found in the tested map\n";
                return test::Result::FAILURE;
            }
            styleRuleCheckResult = checkStyleRule(&(ruleIt->second), &(rule.second));
            if (styleRuleCheckResult != test::Result::SUCCESS) return styleRuleCheckResult;
        }
        return test::Result::SUCCESS;
    }

    test::Result checkStyleDefinitions(const std::list<style::StyleDefinition *> *testedStyleDefinitions,
                                       const std::list<style::StyleDefinition *> *expectedStyleDefinitions) {
        test::Result result;
        std::list<style::StyleDefinition *>::const_iterator testedStyleDefinitionsIt;
        std::list<style::StyleDefinition *>::const_iterator expectedStyleDefinitionsIt;

        if (testedStyleDefinitions == nullptr || expectedStyleDefinitions == nullptr) {
            std::cerr << "One of the two style blocks is nullptr\n";
            return test::Result::FAILURE;
        }

        else if (testedStyleDefinitions == nullptr || testedStyleDefinitions->size() != expectedStyleDefinitions->size()) {
            std::cerr << testedStyleDefinitions->size() << " StyleDefinitions instead of " << expectedStyleDefinitions->size() << " expected\n";
            return test::Result::FAILURE;
        }
        else {
            testedStyleDefinitionsIt = testedStyleDefinitions->cbegin();
            expectedStyleDefinitionsIt = expectedStyleDefinitions->cbegin();
            while (testedStyleDefinitionsIt != testedStyleDefinitions->cend()) {
                result = checkStyleComponentDataList(&(*testedStyleDefinitionsIt)->first, &(*expectedStyleDefinitionsIt)->first);
                if (result != test::Result::SUCCESS) return result;
                result = checkStyleMap(&(*testedStyleDefinitionsIt)->second, &(*expectedStyleDefinitionsIt)->second);
                if (result != test::Result::SUCCESS) return result;
                testedStyleDefinitionsIt++;
                expectedStyleDefinitionsIt++;
            }
        }
        return test::Result::SUCCESS;
    }

    test::Result testDeserializationFromFile(const std::string &fileName, const std::list<style::StyleDefinition *> *expectedStyleDefinitions) {
        int fileNumber = 0;
        int ruleNumber;
        style::config::Config *config = testConfig();
        std::list<style::StyleDefinition *> *styleDefinitions;
        test::Result result;
        std::cout << "Tested style file:\n" << fileName << "\n";
        styleDefinitions = style::StyleDeserializer::deserializeFromFile(fileName, fileNumber, &ruleNumber, config);
        result = checkStyleDefinitions(styleDefinitions, expectedStyleDefinitions);

        for (style::StyleDefinition *component : *styleDefinitions) {
            delete component;
        }
        delete styleDefinitions;
        delete config;
        return result;
    }

    test::Result testDeserialization(const std::string &style, const std::list<style::StyleDefinition *> *expectedStyleDefinitions) {
        int fileNumber = 0;
        int ruleNumber;
        style::config::Config *config = testConfig();
        std::list<style::StyleDefinition *> *styleDefinitions;
        test::Result result;
        std::cout << "Tested style:\n" << style << "\n";
        styleDefinitions = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, config);
        result = checkStyleDefinitions(styleDefinitions, expectedStyleDefinitions);

        for (style::StyleDefinition *component : *styleDefinitions) {
            delete component;
        }
        delete styleDefinitions;
        delete config;
        return result;
    }

    test::Result testSingleRule() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::SelectorType::Class), style::SelectorsRelation::AnyParent));
        expectedData.push_back(std::pair(std::pair("label", style::SelectorType::ElementName), style::SelectorsRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::SelectorType::Identifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 111, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(".container      label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testDirectParent() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::SelectorType::Class), style::SelectorsRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::SelectorType::ElementName), style::SelectorsRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::SelectorType::Identifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 111, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(".container > label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testDirectParentWithoutSpacesAround() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::SelectorType::Class), style::SelectorsRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::SelectorType::ElementName), style::SelectorsRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::SelectorType::Identifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 111, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(".container>label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testRuleNameAndValueStickedToAssignmentColon() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("label", style::SelectorType::ElementName), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("px", style::ValueType::Unit);
        style::StyleValue *styleValue2 = new style::StyleValue("100", style::ValueType::Int);
        styleValue->addChild(styleValue2);
        expectedStyleMap.insert_or_assign("padding", style::StyleRule{styleValue, true, 1, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization("label {padding:100px;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testGlobalModifier() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("hovered", style::SelectorType::Modifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("px", style::ValueType::Unit);
        style::StyleValue *styleValue2 = new style::StyleValue("100", style::ValueType::Int);
        styleValue->addChild(styleValue2);
        expectedStyleMap.insert_or_assign("padding", style::StyleRule{styleValue, true, 10, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(":hovered {padding:100px;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testEmptyBlock() {
        std::list<style::StyleDefinition *> expectedStyleDefinitions = {};
        test::Result result = testDeserialization("a {}", &expectedStyleDefinitions);
        return result;
    }

    test::Result testElementNameSpecificity() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("a", style::SelectorType::ElementName), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("aaaaaa", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 1, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization("a {text-color: #aaaaaa;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testClassSpecificity() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("a", style::SelectorType::Class), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("aaaaaa", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 10, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(".a {text-color: #aaaaaa;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testModifierSpecificity() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("a", style::SelectorType::Modifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("aaaaaa", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 10, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization(":a {text-color: #aaaaaa;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testIdentifierSpecificity() {
        style::SelectorDataList expectedData = style::SelectorDataList();
        style::RulesMap expectedStyleMap = style::RulesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *styleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("a", style::SelectorType::Identifier), style::SelectorsRelation::SameElement));
        styleValue = new style::StyleValue("aaaaaa", style::ValueType::Hex);
        expectedStyleMap.insert_or_assign("text-color", style::StyleRule{styleValue, true, 100, 0, 0});
        styleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {styleDefinition};
        result = testDeserialization("#a {text-color: #aaaaaa;}", &expectedStyleDefinitions);
        delete styleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testDeserializingIdentifierSelector() {
        style::config::Config *config = testConfig();
        style::SelectorDataList *selectors = style::StyleDeserializer::deserializeSelectors("#a", config);
        delete selectors;
        return test::Result::SUCCESS;
    }

    void testsDeserialization(test::Tests *tests) {
        tests->beginTestBlock("Deserialization tests");
        tests->addTest(testSingleRule, "Deserializing a single rule");
        tests->addTest(testDirectParent, "Direct parent");
        tests->addTest(testDirectParentWithoutSpacesAround, "Direct parent without spaces");
        tests->addTest(testRuleNameAndValueStickedToAssignmentColon, "Style name and value sticked to the assignment colon");
        tests->addTest(testGlobalModifier, "Global modifier");
        tests->addTest(testEmptyBlock, "Empty block");
        tests->beginTestBlock("specificities");
        tests->addTest(testElementNameSpecificity, "Element name specificity");
        tests->addTest(testClassSpecificity, "Class specificity");
        tests->addTest(testModifierSpecificity, "Modifier specificity");
        tests->addTest(testIdentifierSpecificity, "Identifier specificity");
        // TODO: add tests ensuring biggest specificity is taken
        tests->endTestBlock();
        tests->beginTestBlock("selectors deserialization");
        tests->addTest(testDeserializingIdentifierSelector, "Deserializing identifier selector");
        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace deserializationTests
