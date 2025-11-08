#include "style_nodes_tests.hpp"

namespace styleNodesTests {
    void displayValue(style::StyleValue *value) {
        if (value == nullptr) {
            std::cerr << "null";
            return;
        }
        std::cerr << value->getValue() << "<" << style::styleValueTypeToString(value->getType()) << ">";
        style::StyleValue *next = value->getNext();
        if (next != nullptr) {
            std::cerr << ",";
            displayValue(next);
        }
        style::StyleValue *child = value->getChild();
        if (child != nullptr) {
            std::cerr << "(";
            while (child != nullptr) {
                displayValue(child);
                child = child->getChild();
            }
            std::cerr << ")";
        }
    }

    bool testValue(bool equal, style::StyleValue *value, style::StyleValue *expected, bool displayDebug) {
        bool result = (value == nullptr || expected == nullptr)
                          ? (value == expected) == equal
                          : ((value->getValue() == expected->getValue() && value->getType() == expected->getType()) == equal);
        if (!result && displayDebug) {
            std::cerr << "attribute -> given\t|\texpected:\n";
            std::cerr << "value -> ";
            displayValue(value);
            std::cerr << "\t|\t";
            displayValue(expected);
            std::cerr << "\n";
        }
        return result;
    }

    bool testRule(bool equal, const style::StyleRule &rule, const style::StyleRule &expected) {
        std::cout << "test if two rules are " << (equal ? "equal" : "not equal") << "\n";
        if ((rule.enabled
             != expected.enabled
             || rule.fileNumber
             != expected.fileNumber
             || rule.ruleNumber
             != expected.ruleNumber
             || rule.specificity
             != expected.specificity
             || testValue(equal, rule.value, expected.value))
            != equal) {
            std::cerr << "rules are " << (equal ? "not equal" : "equal") << "\n";
            std::cerr << "attribute -> given\t|\texpected:\n";
            std::cerr << "-----\n";
            std::cerr << "enabled -> " << rule.enabled << "\t\t|\t" << expected.enabled << "\n";
            std::cerr << "fileNumber -> " << rule.fileNumber << "\t|\t" << expected.fileNumber << "\n";
            std::cerr << "ruleNumber -> " << rule.ruleNumber << "\t|\t" << expected.ruleNumber << "\n";
            std::cerr << "specificity -> " << rule.specificity << "\t|\t" << expected.specificity << "\n";
            std::cerr << "value -> ";
            displayValue(rule.value);
            std::cerr << "\t|\t";
            displayValue(expected.value);
            std::cerr << "\n";
            return false;
        }
        return true;
    }

    test::Result testGetRuleWithoutRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;
        if (result != nullptr) {
            std::cerr << "rule found but shouldn't exist\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testAddAndGetDefaultRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule()}}};
        node->addStyle(style);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 1) {
            std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule()));
        }
        delete node;
        return testResult;
    }

    test::Result testAddDisabledRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 1) {
            std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, false, 3, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testAddCustomRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 1) {
            std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testAddSecondRuleHigherPriority() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 17, 5, 3)}}};
        node->addStyle(style2);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;

        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 2) {
            std::cerr << "expected 2 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            style::StyleValue expectedValue = style::StyleValue("bcd", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 17, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testAddSecondRuleLowerPriority() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        test::Result testResult;

        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 2) {
            std::cerr << "expected 2 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else if (result->second.size() != 1) {
                std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second.front();
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteDisabledRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else if (result->second.size() != 1) {
                std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second.front();
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteRuleWithTwoRulesWithSameNumber() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else if (result->second.size() != 1) {
                std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
                testResult = test::Result::FAILURE;
            }
            else {
                testResult = test::Result::SUCCESS;
            }
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteRulesFromFile() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        style::StyleValue value3 = style::StyleValue("bcd", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style3 = {{"test", {style::StyleRule(&value3, true, 1, 5, 3)}}};
        node->addStyle(style3);
        test::Result testResult;

        int nbRulesDeleted = node->deleteStyleFromFile(5);
        if (nbRulesDeleted != 2) {
            std::cerr << "deleteStyleFromFile returned " << nbRulesDeleted << " instead of 2\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else if (result->second.size() != 1) {
                std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second.front();
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
        delete node;
        return testResult;
    }

    test::Result testUpdateRulePriorityFromFile() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        test::Result testResult;

        node->updateStylePriorityFromFile(5, 7);
        style::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else if (result->second.size() != 1) {
            std::cerr << "expected 1 rule value, got " << result->second.size() << "\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second.front();
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 7, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testClear() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testClearWithDisabledRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testClearMultipleRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("pou", style::StyleValueType::Int);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, false, 7, 2, 18)}}};
        node->addStyle(style2);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testClearMultipleValuesOnOneRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("pou", style::StyleValueType::Int);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 7, 2, 18)}}};
        node->addStyle(style2);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesEmpty() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();

        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesOneRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        int result = node->nbRules();
        delete node;
        if (result != 1) {
            std::cerr << "expected 1 rule, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesOneDisabledRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);
        int result = node->nbRules();
        delete node;
        if (result != 1) {
            std::cerr << "expected 1 rule, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesMultipleRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"aze", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        style::StyleValue value3 = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style3 = {{"poi", {style::StyleRule(&value3, true, 3, 5, 3)}}};
        node->addStyle(style3);
        style::StyleValue value4 = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style4 = {{"ergh", {style::StyleRule(&value4, true, 3, 5, 3)}}};
        node->addStyle(style4);
        style::StyleValue value5 = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style5 = {{"hrg", {style::StyleRule(&value5, true, 3, 5, 3)}}};
        node->addStyle(style5);
        int result = node->nbRules();
        delete node;
        if (result != 5) {
            std::cerr << "expected 5 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesMultipleValuesOnOneRule() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        int result = node->nbRules();
        delete node;
        if (result != 2) {
            std::cerr << "expected 2 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesMultipleValuesOnOneRuleWithOneDisabled() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 3, 5, 3)}}};
        node->addStyle(style2);
        int result = node->nbRules();
        delete node;
        if (result != 2) {
            std::cerr << "expected 2 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesAfterCleared() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testGetOneRuleOfOne() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetOneRuleOfOneDisabled() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetOneRuleOfMultiple() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("oiuy", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetOneRuleWithMultipleValues() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 17, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetLastAddedValueInRuleWithSameSpecificityValues() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 7)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleWithNoMatchingRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 7)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test3", &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleWithNoRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;

        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithListOfRuleNamesWithSameSpecificity() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test", "test2"};
        bool result = node->getRule(ruleNames, &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithListOfRuleNames() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test", "test2"};
        bool result = node->getRule(ruleNames, &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueInEmptyListOfRuleNames() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {};
        bool result = node->getRule(ruleNames, &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithNoMatchingRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test3"};
        bool result = node->getRule(ruleNames, &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithNoRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test"};
        bool result = node->getRule(ruleNames, &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameIsTrue() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        bool result = node->ruleExists("test");
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameIsTrueDisabled() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);
        bool result = node->ruleExists("test");
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameIsFalse() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        bool result = node->ruleExists("test3");
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameWithoutRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;

        bool result = node->ruleExists("test");
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersIsTrue() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->addStyle(style2);

        bool result = node->ruleExists(5, 3);
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersIsFalse() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->addStyle(style2);

        bool result = node->ruleExists(12, 17);
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersWithoutRules() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;

        bool result = node->ruleExists(1, 2);
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetSelectorsEmpty() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (!result->empty()) {
            std::cerr << "getSelectors returned list with " << result->size() << " elements instead of empty list\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testSetAndGetSelector() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 1) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 1\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult =
                test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class}) != result->cend());
        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectors() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("myId", style::StyleComponentType::Identifier);

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 2) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 2\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult = test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class})
                                               != result->cend()
                                               && result->find(style::StyleComponentData{"myId", style::StyleComponentType::Identifier})
                                               != result->cend());
        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectorsSameType() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("mySecondClass", style::StyleComponentType::Class);

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 2) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 2\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult = test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class})
                                               != result->cend()
                                               && result->find(style::StyleComponentData{"mySecondClass", style::StyleComponentType::Class})
                                               != result->cend());
        delete node;
        return testResult;
    }

    test::Result testAddModifier() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 3)}}};
        node->addStyle(style2);
        node->addModifier("myModifier");
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testAddRuleAffectedByModifier() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
        node->addStyle(style2);
        node->addModifier("myModifier");
        node->addRuleAffectedByModifier(5, 4, "myModifier");
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testEnableModifier() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
        node->addStyle(style2);
        node->addModifier("myModifier");
        node->addRuleAffectedByModifier(5, 4, "myModifier");
        node->setModifierState("myModifier", true);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testDisableModifier() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
        node->addStyle(style2);
        node->addModifier("myModifier");
        node->addRuleAffectedByModifier(5, 4, "myModifier");
        node->setModifierState("myModifier", true);
        node->setModifierState("myModifier", false);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testToggleRuleDisable() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        node->toggleRule(5, 3);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testToggleRuleReenable() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        node->toggleRule(5, 3);
        node->toggleRule(5, 3);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testToggleRuleForceEnable() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        node->toggleRule(5, 3, true);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testToggleRuleForceDisable() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        node->toggleRule(5, 3, false);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testToggleRuleForceReenable() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        node->toggleRule(5, 3, false);
        node->toggleRule(5, 3, true);
        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, false, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testFontsPathEmpty() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "/");
        delete node;
        return testResult;
    }

    test::Result testFontsPath() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->setFontsPath("path");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathWithTrailingSlash() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->setFontsPath("path/");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathOverride() {
        style::elementStyle::StyleNode *node = new style::elementStyle::StyleNode();
        test::Result testResult;
        node->setFontsPath("path");
        node->setFontsPath("path2");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path2/");
        delete node;
        return testResult;
    }

    void testsStyleNodes(test::Tests *tests) {
        tests->beginTestBlock("Tests style nodes");
        tests->beginTestBlock("Add rules");
        tests->addTest(testGetRuleWithoutRules, "get rule without rules");
        tests->addTest(testAddAndGetDefaultRule, "add and get default rule");
        tests->addTest(testAddDisabledRule, "add disabled rule");
        tests->addTest(testAddCustomRule, "add custom rule");
        tests->addTest(testAddSecondRuleHigherPriority, "add second rule with higher priority");
        tests->addTest(testAddSecondRuleLowerPriority, "add second rule with lower priority");
        tests->endTestBlock();
        tests->beginTestBlock("Delete rules");
        tests->addTest(testDeleteRule, "delete rule");
        tests->addTest(testDeleteDisabledRule, "delete disabled rule");
        tests->addTest(testDeleteRuleWithTwoRulesWithSameNumber, "delete rule with two rules with same number");
        tests->addTest(testDeleteRulesFromFile, "delete rules from file");
        tests->addTest(testUpdateRulePriorityFromFile, "update rule priority from file");
        tests->addTest(testClear, "clear");
        tests->addTest(testClearWithDisabledRule, "clear with disabled rule");
        tests->addTest(testClearMultipleRules, "clear mutliple rules");
        tests->addTest(testClearMultipleValuesOnOneRule, "clear multiple values on one rule");
        tests->endTestBlock();
        tests->beginTestBlock("Get nb rules");
        tests->addTest(testNbRulesEmpty, "nb rules empty");
        tests->addTest(testNbRulesOneRule, "nb rules one rule");
        tests->addTest(testNbRulesOneDisabledRule, "nb rules one disabled rule");
        tests->addTest(testNbRulesMultipleRules, "nb rules multiple rules");
        tests->addTest(testNbRulesMultipleValuesOnOneRule, "nb rules multiple values on one rule");
        tests->addTest(testNbRulesMultipleValuesOnOneRuleWithOneDisabled, "nb rules multiple values on one rule with one disabled");
        tests->addTest(testNbRulesAfterCleared, "nb rules after cleared");
        tests->endTestBlock();
        tests->beginTestBlock("get rule values");
        tests->addTest(testGetOneRuleOfOne, "get one rule of one");
        tests->addTest(testGetOneRuleOfOneDisabled, "get one rule of one disabled");
        tests->addTest(testGetOneRuleOfMultiple, "get one rule of multiple");
        tests->addTest(testGetOneRuleWithMultipleValues, "get one rule with multiple values");
        tests->addTest(testGetLastAddedValueInRuleWithSameSpecificityValues, "get last added value in rule with same specificity values");
        tests->addTest(testGetRuleWithNoMatchingRules, "get rule with no matching rules");
        tests->addTest(testGetRuleWithNoRules, "get rule with no rules");
        tests->addTest(testGetRuleValueWithListOfRuleNamesWithSameSpecificity, "get rule value with list of rule names with same specificity");
        tests->addTest(testGetRuleValueWithListOfRuleNames, "get rule value with list of rule names");
        tests->addTest(testGetRuleValueInEmptyListOfRuleNames, "get rule value in empty list of rule names");
        tests->addTest(testGetRuleValueWithNoMatchingRules, "get rule value with no matching rules");
        tests->addTest(testGetRuleValueWithNoRules, "get rule value with no rules");
        tests->endTestBlock();
        tests->beginTestBlock("check if rules exists");
        tests->addTest(testRuleExistsWithRuleNameIsTrue, "rule exists with rule name is true");
        tests->addTest(testRuleExistsWithRuleNameIsTrueDisabled, "rule exists with rule name is true disabled");
        tests->addTest(testRuleExistsWithRuleNameIsFalse, "rule exists with rule name is false");
        tests->addTest(testRuleExistsWithRuleNameWithoutRules, "rule exists with rule name without rules");
        tests->addTest(testRuleExistsWithNumbersIsTrue, "rule exists with numbers is true");
        tests->addTest(testRuleExistsWithNumbersIsFalse, "rule exists with numbers is false");
        tests->addTest(testRuleExistsWithNumbersWithoutRules, "rule exists with numbers without rules");
        tests->endTestBlock();
        tests->beginTestBlock("get and set selectors");
        tests->addTest(testGetSelectorsEmpty, "get selectors empty");
        tests->addTest(testSetAndGetSelector, "set and get selector");
        tests->addTest(testSetAndGetMultipleSelectors, "set and get multiple selectors");
        tests->addTest(testSetAndGetMultipleSelectorsSameType, "set and get multiple selectors of same type");
        tests->endTestBlock();
        tests->beginTestBlock("modifiers");
        tests->addTest(testAddModifier, "add modifier");
        tests->addTest(testAddRuleAffectedByModifier, "add rule affected by modifier");
        tests->addTest(testEnableModifier, "enable modifier");
        tests->addTest(testDisableModifier, "disable modifier");
        tests->endTestBlock();
        tests->beginTestBlock("toggle rules");
        tests->addTest(testToggleRuleDisable, "toggle rule disable");
        tests->addTest(testToggleRuleReenable, "toggle rule reenable");
        tests->addTest(testToggleRuleForceEnable, "toggle rule force enable");
        tests->addTest(testToggleRuleForceDisable, "toggle rule force disable");
        tests->addTest(testToggleRuleForceReenable, "toggle rule force reenable");
        tests->endTestBlock();
        tests->beginTestBlock("fonts path");
        tests->addTest(testFontsPathEmpty, "fonts path empty");
        tests->addTest(testFontsPath, "fonts path");
        tests->addTest(testFontsPathWithTrailingSlash, "fonts path with trailing slash");
        tests->addTest(testFontsPathOverride, "fonts path override");
        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace styleNodesTests