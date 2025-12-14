#include "deserialization_tests.hpp"

namespace deserializationTests {

    test::Result checkStyleComponentDataList(const style::StyleComponentDataList *testedData, const style::StyleComponentDataList *expectedData) {
        if (testedData == nullptr && expectedData == nullptr) return test::Result::SUCCESS;
        if ((testedData == nullptr && expectedData != nullptr)
            || (testedData != nullptr && expectedData == nullptr)
            || (testedData->size() != expectedData->size())) {
            std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
            return test::Result::FAILURE;
        }
        style::StyleComponentDataList::const_iterator testedDataListIt = testedData->cbegin();
        style::StyleComponentDataList::const_iterator expectedDataListIt = expectedData->cbegin();
        while (testedDataListIt != testedData->cend()) {
            if (testedDataListIt->second != expectedDataListIt->second) {
                std::cerr
                    << styleRelationToString(testedDataListIt->second)
                    << " instead of relation "
                    << styleRelationToString(expectedDataListIt->second)
                    << "\n";
                return test::Result::FAILURE;
            }
            if (testedDataListIt->first.first != expectedDataListIt->first.first) {
                std::cerr << testedDataListIt->first.first << " instead of name " << expectedDataListIt->first.first << "\n";
                return test::Result::FAILURE;
            }
            if (testedDataListIt->first.second != expectedDataListIt->first.second) {
                std::cerr
                    << styleComponentTypeToString(testedDataListIt->first.second)
                    << " instead of type "
                    << styleComponentTypeToString(expectedDataListIt->first.second)
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
                << styleValueTypeToString(testedValue->type())
                << "', expected '"
                << styleValueTypeToString(expectedValue->type())
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

    test::Result checkStyleMap(const style::StyleValuesMap *testedStyleMap, const style::StyleValuesMap *expectedStyleMap) {
        style::StyleValuesMap::const_iterator ruleIt;
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
        std::list<style::StyleDefinition *> *StyleDefinitions;
        test::Result result;
        std::cout << "Tested style file:\n" << fileName << "\n";
        StyleDefinitions = style::StyleDeserializer::deserializeFromFile(fileName, fileNumber, &ruleNumber, config);
        result = checkStyleDefinitions(StyleDefinitions, expectedStyleDefinitions);

        for (style::StyleDefinition *component : *StyleDefinitions) {
            delete component;
        }
        delete StyleDefinitions;
        delete config;
        return result;
    }

    test::Result testDeserialization(const std::string &style, const std::list<style::StyleDefinition *> *expectedStyleDefinitions) {
        int fileNumber = 0;
        int ruleNumber;
        style::config::Config *config = testConfig();
        std::list<style::StyleDefinition *> *StyleDefinitions;
        test::Result result;
        std::cout << "Tested style:\n" << style << "\n";
        StyleDefinitions = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, config);
        result = checkStyleDefinitions(StyleDefinitions, expectedStyleDefinitions);

        for (style::StyleDefinition *component : *StyleDefinitions) {
            delete component;
        }
        delete StyleDefinitions;
        delete config;
        return result;
    }

    template <typename T>
    test::Result checkDeserializationError(const std::string &style) {
        int fileNumber = 0;
        int ruleNumber;
        style::config::Config *config = testConfig();
        test::Result result;
        std::list<style::StyleDefinition *> *StyleDefinitions;
        std::cout << "Tested style:\n" << style << "\n";
        try {
            StyleDefinitions = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, config);
            result = test::Result::FAILURE;

            for (style::StyleDefinition *component : *StyleDefinitions) {
                delete component;
            }
            delete StyleDefinitions;
            delete config;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) result = test::Result::SUCCESS;
            else {
                std::cerr << "Throwed invalid exception '" << exception.what() << "\n";
                result = test::Result::FAILURE;
            }
        }
        catch (...) {
            std::cerr << "Throwed invalid exception who were not a subclass of std::exception\n";
            result = test::Result::FAILURE;
        }
        return result;
    }

    test::Result testSingleRule() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *StyleDefinition;
        style::config::Config *config = testConfig();
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::AnyParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        StyleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {StyleDefinition};
        result = testDeserialization(".container      label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete StyleDefinition;
        delete config;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testDirectParent() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *StyleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        StyleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {StyleDefinition};
        result = testDeserialization(".container > label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete StyleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testDirectParentWithoutSpacesAround() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *StyleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        StyleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {StyleDefinition};
        result = testDeserialization(".container>label#red{text-color : #ff0000;}", &expectedStyleDefinitions);
        delete StyleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testRuleNameAndValueStickedToAssignmentColon() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *StyleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("px", style::StyleValueType::Unit);
        style::StyleValue *styleValue2 = new style::StyleValue("100", style::StyleValueType::Int);
        styleValue->addChild(styleValue2);
        expectedStyleMap["padding"] = style::StyleRule{styleValue, true, 1, 0, 0};
        StyleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {StyleDefinition};
        result = testDeserialization("label {padding:100px;}", &expectedStyleDefinitions);
        delete StyleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testGlobalModifier() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleDefinition *StyleDefinition;
        std::list<style::StyleDefinition *> expectedStyleDefinitions;
        test::Result result;

        expectedData.push_back(std::pair(std::pair("hovered", style::StyleComponentType::Modifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("px", style::StyleValueType::Unit);
        style::StyleValue *styleValue2 = new style::StyleValue("100", style::StyleValueType::Int);
        styleValue->addChild(styleValue2);
        expectedStyleMap["padding"] = style::StyleRule{styleValue, true, 10, 0, 0};
        StyleDefinition = new style::StyleDefinition(expectedData, expectedStyleMap);
        expectedStyleDefinitions = {StyleDefinition};
        result = testDeserialization(":hovered {padding:100px;}", &expectedStyleDefinitions);
        delete StyleDefinition;
        expectedStyleMap.clear();
        expectedData.clear();
        return result;
    }

    test::Result testEmptyBlock() {
        std::list<style::StyleDefinition *> expectedStyleDefinitions = {};
        test::Result result = testDeserialization("a {}", &expectedStyleDefinitions);
        return result;
    }

    test::Result testMissingSemiColonAfterAssignment() {
        return checkDeserializationError<style::MissingTokenException>(".container      label#red{text-color : #ff0000}");
    }

    test::Result testMissingStyleValue() {
        return checkDeserializationError<style::MalformedExpressionException>(".container>label#red{text-color;}");
    }

    test::Result testMissingRuleName() { return checkDeserializationError<style::MalformedExpressionException>(".container>label#red{: value}"); }

    test::Result testMissingRuleNameAndValue() { return checkDeserializationError<style::MalformedExpressionException>(".container>label#red{:}"); }

    test::Result testMissingBlockDeclaration() { return checkDeserializationError<style::MalformedExpressionException>("{text-color: #ffffff;}"); }

    test::Result testMissingBlockDeclarationComponentBeforeDirectParentRelation() {
        return checkDeserializationError<style::MissingTokenException>(">label#red{text-color: #ffffff;}");
    }

    void testsDeserialization(test::Tests *tests) {
        tests->beginTestBlock("Deserialization tests");
        tests->addTest(testSingleRule, "Deserializing a single rule");
        tests->addTest(testDirectParent, "Direct parent");
        tests->addTest(testDirectParentWithoutSpacesAround, "Direct parent without spaces");
        tests->addTest(testRuleNameAndValueStickedToAssignmentColon, "Style name and value sticked to the assignment colon");
        tests->addTest(testGlobalModifier, "Global modifier");
        tests->addTest(testEmptyBlock, "Empty block");
        tests->addTest(testMissingSemiColonAfterAssignment, "Missing semi-colon after assignment");
        tests->addTest(testMissingStyleValue, "Missing style value");
        tests->addTest(testMissingRuleName, "Missing style name");
        tests->addTest(testMissingRuleNameAndValue, "Missing style name and value");
        tests->addTest(testMissingBlockDeclaration, "Missing block declaration");
        tests->addTest(testMissingBlockDeclarationComponentBeforeDirectParentRelation,
                       "Missing block declaration component before direct parent relation");
        tests->endTestBlock();
    }

} // namespace deserializationTests
