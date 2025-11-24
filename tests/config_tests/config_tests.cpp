#include "config_tests.hpp"

namespace configTests {

    template <typename T>
    test::Result checkConfigError(const style::config::Config *config) {
        test::Result result;
        try {
            style::config::configChecker(config);
            result = test::Result::FAILURE;
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

    test::Result checkConfigValid(const style::config::Config *config) {
        test::Result result;
        try {
            style::config::configChecker(config);
            result = test::Result::SUCCESS;
        }
        catch (std::exception &exception) {
            std::cerr << "Throwed invalid exception '" << exception.what() << "\n";
            result = test::Result::FAILURE;
        }
        catch (...) {
            std::cerr << "Throwed invalid exception who were not a subclass of std::exception\n";
            result = test::Result::FAILURE;
        }
        return result;
    }

    test::Result testInvalidRuleNameWithSpaces() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"an invalid rule", {node}}}, {}};
        test::Result result = checkConfigError<style::config::InvalidStyleConfigException>(config);
        delete config;
        return result;
    }

    test::Result testInvalidRuleNameWithSpecialFirstCharacter() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"#anInvalidRule", {node}}}, {}};
        test::Result result = checkConfigError<style::config::InvalidStyleConfigException>(config);
        delete config;
        return result;
    }

    test::Result testInvalidRuleNameWithNumericFirstCharacter() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"1anInvalidRule", {node}}}, {}};
        test::Result result = checkConfigError<style::config::InvalidStyleConfigException>(config);
        delete config;
        return result;
    }

    test::Result testInvalidRuleNameWithSpecialCharacter() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"an&InvalidRule", {node}}}, {}};
        test::Result result = checkConfigError<style::config::InvalidStyleConfigException>(config);
        delete config;
        return result;
    }

    test::Result testValidRuleName() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"rule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    test::Result testValidRuleNameWithUpperCase() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"aValidRule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    test::Result testRuleInvalidToken() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::NullRoot);
        const style::config::Config *config = new style::config::Config{{{"anInvalidRule", {node}}}, {}};
        test::Result result = checkConfigError<style::config::InvalidStyleConfigException>(config);
        delete config;
        return result;
    }

    test::Result testRuleValidToken() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Int);
        const style::config::Config *config = new style::config::Config{{{"aValidRule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    test::Result testRuleWithFunctionTokenWithChilds() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Function);
        node->addChild(new style::config::ConfigRuleNode(style::Token::String));
        const style::config::Config *config = new style::config::Config{{{"aValidRule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    test::Result testRuleWithTupleTokenWithChilds() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::Tuple);
        node->addChild(new style::config::ConfigRuleNode(style::Token::String));
        const style::config::Config *config = new style::config::Config{{{"aValidRule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    test::Result testRuleWithNonNestableTokenWithChilds() {
        style::config::ConfigRuleNode *node = new style::config::ConfigRuleNode(style::Token::EnumValue);
        node->addChild(new style::config::ConfigRuleNode(style::Token::String));
        const style::config::Config *config = new style::config::Config{{{"anInvalidRule", {node}}}, {}};
        test::Result result = checkConfigValid(config);
        delete config;
        return result;
    }

    void configTests(test::Tests *tests) {
        tests->beginTestBlock("Config tests");
        tests->addTest(testInvalidRuleNameWithSpaces, "Invalid rule name with spaces");
        tests->addTest(testInvalidRuleNameWithSpecialFirstCharacter, "Invalid rule name with special first character");
        tests->addTest(testInvalidRuleNameWithNumericFirstCharacter, "Invalid rule name with numeric first character");
        tests->addTest(testInvalidRuleNameWithSpecialCharacter, "Invalid rule name with special character");
        tests->addTest(testValidRuleName, "Valid rule name");
        tests->addTest(testValidRuleNameWithUpperCase, "Valid rule name with upper case");
        tests->addTest(testRuleInvalidToken, "Invalid rule token");
        tests->addTest(testRuleValidToken, "Valid rule token");
        tests->addTest(testRuleWithFunctionTokenWithChilds, "Rule with function token with childs");
        tests->addTest(testRuleWithTupleTokenWithChilds, "Rule with tuple token with childs");
        tests->addTest(testRuleWithNonNestableTokenWithChilds, "Rule with non nestable token with childs");
        tests->endTestBlock();
    }

} // namespace configTests
