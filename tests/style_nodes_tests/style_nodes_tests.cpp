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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule()}}};
        node->addStyle(style);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->addStyle(style);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 17, 5, 3)}}};
        node->addStyle(style2);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 1, 5, 3)}}};
        node->addStyle(style2);
        style::StyleValue value3 = style::StyleValue("bcd", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style3 = {{"test", {style::StyleRule(&value3, true, 1, 5, 3)}}};
        node->addStyle(style3);
        test::Result testResult;

        int nbRulesDeleted = node->deleteStyleFromFile(5);
        if (nbRulesDeleted != 2) {
            std::cerr << "deleteStyleFromFile returned " << nbRulesDeleted << " instead of 2\n";
            testResult = test::Result::FAILURE;
        }
        else {
            gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        test::Result testResult;

        node->updateStylePriorityFromFile(5, 7);
        gui::elementStyle::AppliedStyleMap::const_iterator result = node->getStyle().find("test");
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("pou", style::StyleValueType::Int);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, false, 7, 2, 18)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("pou", style::StyleValueType::Int);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 7, 2, 18)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();

        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesOneRule() {
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"aze", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->addStyle(style2);
        style::StyleValue value3 = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style3 = {{"poi", {style::StyleRule(&value3, true, 3, 5, 3)}}};
        node->addStyle(style3);
        style::StyleValue value4 = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style4 = {{"ergh", {style::StyleRule(&value4, true, 3, 5, 3)}}};
        node->addStyle(style4);
        style::StyleValue value5 = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style5 = {{"hrg", {style::StyleRule(&value5, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, false, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("oiuy", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 17, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 7)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 7)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->addStyle(style);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style2 = {{"test", {style::StyleRule(&value2, true, 15, 5, 4)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        gui::elementStyle::AppliedStyleMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
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
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "/");
        delete node;
        return testResult;
    }

    test::Result testFontsPath() {
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        node->setFontsPath("path");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathWithTrailingSlash() {
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
        test::Result testResult;
        node->setFontsPath("path/");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathOverride() {
        gui::elementStyle::StyleNode *node = new gui::elementStyle::StyleNode();
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
        tests->runTest(testGetRuleWithoutRules, "get rule without rules");
        tests->runTest(testAddAndGetDefaultRule, "add and get default rule");
        tests->runTest(testAddDisabledRule, "add disabled rule");
        tests->runTest(testAddCustomRule, "add custom rule");
        tests->runTest(testAddSecondRuleHigherPriority, "add second rule with higher priority");
        tests->runTest(testAddSecondRuleLowerPriority, "add second rule with lower priority");
        tests->endTestBlock();
        tests->beginTestBlock("Delete rules");
        tests->runTest(testDeleteRule, "delete rule");
        tests->runTest(testDeleteDisabledRule, "delete disabled rule");
        tests->runTest(testDeleteRuleWithTwoRulesWithSameNumber, "delete rule with two rules with same number");
        tests->runTest(testDeleteRulesFromFile, "delete rules from file");
        tests->runTest(testUpdateRulePriorityFromFile, "update rule priority from file");
        tests->runTest(testClear, "clear");
        tests->runTest(testClearWithDisabledRule, "clear with disabled rule");
        tests->runTest(testClearMultipleRules, "clear mutliple rules");
        tests->runTest(testClearMultipleValuesOnOneRule, "clear multiple values on one rule");
        tests->endTestBlock();
        tests->beginTestBlock("Get nb rules");
        tests->runTest(testNbRulesEmpty, "nb rules empty");
        tests->runTest(testNbRulesOneRule, "nb rules one rule");
        tests->runTest(testNbRulesOneDisabledRule, "nb rules one disabled rule");
        tests->runTest(testNbRulesMultipleRules, "nb rules multiple rules");
        tests->runTest(testNbRulesMultipleValuesOnOneRule, "nb rules multiple values on one rule");
        tests->runTest(testNbRulesMultipleValuesOnOneRuleWithOneDisabled, "nb rules multiple values on one rule with one disabled");
        tests->runTest(testNbRulesAfterCleared, "nb rules after cleared");
        tests->endTestBlock();
        tests->beginTestBlock("get rule values");
        tests->runTest(testGetOneRuleOfOne, "get one rule of one");
        tests->runTest(testGetOneRuleOfOneDisabled, "get one rule of one disabled");
        tests->runTest(testGetOneRuleOfMultiple, "get one rule of multiple");
        tests->runTest(testGetOneRuleWithMultipleValues, "get one rule with multiple values");
        tests->runTest(testGetLastAddedValueInRuleWithSameSpecificityValues, "get last added value in rule with same specificity values");
        tests->runTest(testGetRuleWithNoMatchingRules, "get rule with no matching rules");
        tests->runTest(testGetRuleWithNoRules, "get rule with no rules");
        tests->runTest(testGetRuleValueWithListOfRuleNamesWithSameSpecificity, "get rule value with list of rule names with same specificity");
        tests->runTest(testGetRuleValueWithListOfRuleNames, "get rule value with list of rule names");
        tests->runTest(testGetRuleValueInEmptyListOfRuleNames, "get rule value in empty list of rule names");
        tests->runTest(testGetRuleValueWithNoMatchingRules, "get rule value with no matching rules");
        tests->runTest(testGetRuleValueWithNoRules, "get rule value with no rules");
        tests->endTestBlock();
        tests->beginTestBlock("check if rules exists");
        tests->runTest(testRuleExistsWithRuleNameIsTrue, "rule exists with rule name is true");
        tests->runTest(testRuleExistsWithRuleNameIsTrueDisabled, "rule exists with rule name is true disabled");
        tests->runTest(testRuleExistsWithRuleNameIsFalse, "rule exists with rule name is false");
        tests->runTest(testRuleExistsWithRuleNameWithoutRules, "rule exists with rule name without rules");
        tests->runTest(testRuleExistsWithNumbersIsTrue, "rule exists with numbers is true");
        tests->runTest(testRuleExistsWithNumbersIsFalse, "rule exists with numbers is false");
        tests->runTest(testRuleExistsWithNumbersWithoutRules, "rule exists with numbers without rules");
        tests->endTestBlock();
        tests->beginTestBlock("get and set selectors");
        tests->runTest(testGetSelectorsEmpty, "get selectors empty");
        tests->runTest(testSetAndGetSelector, "set and get selector");
        tests->runTest(testSetAndGetMultipleSelectors, "set and get multiple selectors");
        tests->runTest(testSetAndGetMultipleSelectorsSameType, "set and get multiple selectors of same type");
        tests->endTestBlock();
        tests->beginTestBlock("modifiers");
        tests->runTest(testAddModifier, "add modifier");
        tests->runTest(testAddRuleAffectedByModifier, "add rule affected by modifier");
        tests->runTest(testEnableModifier, "enable modifier");
        tests->runTest(testDisableModifier, "disable modifier");
        tests->endTestBlock();
        tests->beginTestBlock("toggle rules");
        tests->runTest(testToggleRuleDisable, "toggle rule disable");
        tests->runTest(testToggleRuleReenable, "toggle rule reenable");
        tests->runTest(testToggleRuleForceEnable, "toggle rule force enable");
        tests->runTest(testToggleRuleForceDisable, "toggle rule force disable");
        tests->runTest(testToggleRuleForceReenable, "toggle rule force reenable");
        tests->endTestBlock();
        tests->beginTestBlock("fonts path");
        tests->runTest(testFontsPathEmpty, "fonts path empty");
        tests->runTest(testFontsPath, "fonts path");
        tests->runTest(testFontsPathWithTrailingSlash, "fonts path with trailing slash");
        tests->runTest(testFontsPathOverride, "fonts path override");
        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace styleNodesTests