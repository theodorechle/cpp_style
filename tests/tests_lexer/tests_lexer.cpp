#include "tests_lexer.hpp"
#include "../../src/lexer.hpp"
#include "../test_config.hpp"

namespace testsLexer {
    test::Result testLexer(bool equal, const std::string &expression, const style::lexer::LexerNode *expected) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expression << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ":\n";
        try {
            style::lexer::LexerNode *result = style::lexer::Lexer().lexe(expression, config);
            style::lexer::LexerNode *n = result;
            std::cout << "result:\n";
            n->displayNexts();
            while (n != nullptr) {
                if ((expected == nullptr || !(style::lexer::areSameNodes(n, expected))) == equal) {
                    delete result;
                    delete config;
                    return test::Result::FAILURE;
                }
                n = n->next();
                expected = expected->next();
            }
            if (expected == nullptr) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
            delete config;
        }
        catch (const std::exception &e) {
            testResult = test::Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        return testResult;
    }

    template <typename T>
    test::Result testLexerException(const std::string &expression) {
        style::config::Config *config = testConfig();
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expression << "\n'\n raises an exception : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::lexer::LexerNode *tokens = nullptr;
        try {
            tokens = style::lexer::Lexer().lexe(expression, config);
#ifdef DEBUG
            tokens->debugDisplay();
#endif
            testResult = test::Result::FAILURE;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) testResult = test::Result::SUCCESS;
            else {
                testResult = test::Result::ERROR;
                std::cerr << "Error : " << exception.what();
            }
        }
        delete config;
        delete tokens;
        std::cout << "\n";
        return testResult;
    }

    test::Result testLexingEmpty() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = nullptr;
        result = testLexer(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleSpace() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Space);
        result = testLexer(true, " ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleSpaces() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Space);
        result = testLexer(true, "   ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingTabulation() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Space);
        result = testLexer(true, "\t", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleLineBreak() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::LineBreak);
        result = testLexer(true, "\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleLineBreaks() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::LineBreak);
        result = testLexer(true, "\n\n\n\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveInteger() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Int, "1");
        result = testLexer(true, "1", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerZero() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Int, "0");
        result = testLexer(true, "0", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeInteger() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Int, "-5");
        result = testLexer(true, "-5", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerWithMultipleChars() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Int, "2679");
        result = testLexer(true, "2679", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveFloat() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, "26.3");
        result = testLexer(true, "26.3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatZero() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, "0.");
        result = testLexer(true, "0.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloat() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, "-12.6");
        result = testLexer(true, "-12.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoIntegralPart() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, ".3");
        result = testLexer(true, ".3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoDecimalPart() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, "5.");
        result = testLexer(true, "5.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoIntegerPart() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Float, "-.6");
        result = testLexer(true, "-.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoParts() { return testLexerException<style::lexer::LexerException>("-."); }

    test::Result testLexingSemiColon() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::SemiColon);
        result = testLexer(true, ";", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingColon() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Colon);
        result = testLexer(true, ":", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingComma() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Comma);
        result = testLexer(true, ",", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningCurlyBracket() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::OpeningCurlyBracket);
        result = testLexer(true, "{", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingCurlyBracket() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::ClosingCurlyBracket);
        result = testLexer(true, "}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningParenthesis() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::OpeningParenthesis);
        result = testLexer(true, "(", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingParenthesis() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::ClosingParenthesis);
        result = testLexer(true, ")", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingGreatherThan() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::GreaterThan);
        result = testLexer(true, ">", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSharp() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Sharp);
        result = testLexer(true, "#", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingDot() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Dot);
        result = testLexer(true, ".", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingAt() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::At);
        result = testLexer(true, "@", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStar() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::Star);
        result = testLexer(true, "*", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawName() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::RawName, "test");
        result = testLexer(true, "test", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithHyphen() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::RawName, "test-a");
        result = testLexer(true, "test-a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithNumber() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::RawName, "test2");
        result = testLexer(true, "test2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithUnderscore() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::RawName, "test_2");
        result = testLexer(true, "test_2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameStartingWithNumber() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::RawName, "2a");
        result = testLexer(true, "2a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMinusSign() { return testLexerException<style::lexer::LexerException>("-"); }

    test::Result testLexingStringDoubleQuotes() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::String, "value");
        result = testLexer(true, "\"value\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringSingleQuotes() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::String, "value");
        result = testLexer(true, "'value'", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringUnclosedDoubleQuotes() { return testLexerException<style::lexer::LexerException>("\"value"); }

    test::Result testLexingStringUnclosedSingleQuotes() { return testLexerException<style::lexer::LexerException>("'value"); }

    test::Result testLexingStringEmptyDoubleQuotes() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::String, "");
        result = testLexer(true, "\"\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringEmptySingleQuotes() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::String, "");
        result = testLexer(true, "''", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingImport() {
        style::lexer::LexerNode *rootExpected;
        test::Result result;

        rootExpected = new style::lexer::LexerNode(style::lexer::Token::At);
        rootExpected->appendNext(new style::lexer::LexerNode(style::lexer::Token::RawName, "import"))
            ->appendNext(new style::lexer::LexerNode(style::lexer::Token::Space))
            ->appendNext(new style::lexer::LexerNode(style::lexer::Token::String, "test"))
            ->appendNext(new style::lexer::LexerNode(style::lexer::Token::SemiColon));
        result = testLexer(true, "@import \"test\";", rootExpected);
        delete rootExpected;
        return result;
    }

    void testsLexer(test::Tests *tests) {
        tests->beginTestBlock("Tests lexer");
        tests->beginTestBlock("White spaces");
        tests->addTest(testLexingEmpty, "Empty");
        tests->addTest(testLexingSingleSpace, "Single space");
        tests->addTest(testLexingMultipleSpaces, "Multiple spaces");
        tests->addTest(testLexingSingleLineBreak, "Single line break");
        tests->addTest(testLexingMultipleLineBreaks, "Multiple line breaks");
        tests->addTest(testLexingTabulation, "Tabulation");
        tests->endTestBlock();

        tests->beginTestBlock("Integers");
        tests->addTest(testLexingPositiveInteger, "Positive integer");
        tests->addTest(testLexingIntegerZero, "Integer 0");
        tests->addTest(testLexingNegativeInteger, "Negative integer");
        tests->addTest(testLexingIntegerWithMultipleChars, "Integer with multiple chars");
        tests->endTestBlock();

        tests->beginTestBlock("Floats");
        tests->addTest(testLexingPositiveFloat, "Positive float");
        tests->addTest(testLexingFloatZero, "Float 0");
        tests->addTest(testLexingNegativeFloat, "Negative float");
        tests->addTest(testLexingFloatNoIntegralPart, "Float no integral part");
        tests->addTest(testLexingFloatNoDecimalPart, "Float no decimal part");
        tests->addTest(testLexingNegativeFloatNoIntegerPart, "Negative float no decimal part");
        tests->addTest(testLexingNegativeFloatNoParts, "Negative float no parts");
        tests->endTestBlock();

        tests->beginTestBlock("Reserved characters");
        tests->addTest(testLexingSemiColon, "Semicolon");
        tests->addTest(testLexingColon, "Colon");
        tests->addTest(testLexingComma, "Comma");
        tests->addTest(testLexingOpeningCurlyBracket, "Opening curly bracket");
        tests->addTest(testLexingClosingCurlyBracket, "Closing curly bracket");
        tests->addTest(testLexingGreatherThan, "Greater than");
        tests->addTest(testLexingSharp, "Sharp");
        tests->addTest(testLexingDot, "Dot");
        tests->addTest(testLexingAt, "At");
        tests->addTest(testLexingStar, "Star");
        tests->endTestBlock();

        tests->beginTestBlock("Raw names");
        tests->addTest(testLexingRawName, "Raw name");
        tests->addTest(testLexingRawNameWithHyphen, "Raw name with hyphen");
        tests->addTest(testLexingRawNameWithNumber, "Raw name with number");
        tests->addTest(testLexingRawNameWithUnderscore, "Raw name with underscore");
        tests->addTest(testLexingRawNameStartingWithNumber, "Raw name with starting with number");
        tests->addTest(testLexingMinusSign, "Minus sign");
        tests->endTestBlock();

        tests->beginTestBlock("Strings");
        tests->addTest(testLexingStringDoubleQuotes, "String double quotes");
        tests->addTest(testLexingStringSingleQuotes, "String single quotes");
        tests->addTest(testLexingStringUnclosedDoubleQuotes, "String unclosed double quotes");
        tests->addTest(testLexingStringUnclosedSingleQuotes, "String unclosed single quotes");
        tests->addTest(testLexingStringEmptyDoubleQuotes, "String empty double quotes");
        tests->addTest(testLexingStringEmptySingleQuotes, "String empty single quotes");
        tests->endTestBlock();

        tests->beginTestBlock("Multiple tokens");
        tests->addTest(testLexingImport, "Import");
        tests->endTestBlock();

        tests->endTestBlock();
    }
} // namespace testsLexer
