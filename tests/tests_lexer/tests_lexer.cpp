#include "tests_lexer.hpp"

namespace testsLexer {
    test::Result testLexer(bool equal, const std::string &expression, const style::DeserializationNode *expected) {
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expression << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *result = style::Lexer().lexe(expression);
            style::DeserializationNode *n = result;
            std::cout << "result:\n";
            n->displayNexts();
            while (n != nullptr) {
                if ((expected == nullptr || !(*n == *expected)) == equal) {
                    delete result;
                    return test::Result::FAILURE;
                }
                n = n->next();
                expected = expected->next();
            }
            if (expected == nullptr) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
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
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expression << "\n'\n raises an exception : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        try {
            tokens = style::Lexer().lexe(expression);
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
        delete tokens;
        std::cout << "\n";
        return testResult;
    }

    test::Result testLexingEmpty() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = nullptr;
        result = testLexer(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleSpace() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Space);
        result = testLexer(true, " ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleSpaces() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Space);
        result = testLexer(true, "   ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingTabulation() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Space);
        result = testLexer(true, "\t", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleLineBreak() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::LineBreak);
        result = testLexer(true, "\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleLineBreaks() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::LineBreak);
        result = testLexer(true, "\n\n\n\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveInteger() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Int, "1");
        result = testLexer(true, "1", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerZero() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Int, "0");
        result = testLexer(true, "0", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeInteger() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Int, "-5");
        result = testLexer(true, "-5", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerWithMultipleChars() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Int, "2679");
        result = testLexer(true, "2679", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveFloat() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, "26.3");
        result = testLexer(true, "26.3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatZero() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, "0.");
        result = testLexer(true, "0.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloat() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, "-12.6");
        result = testLexer(true, "-12.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoIntegralPart() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, ".3");
        result = testLexer(true, ".3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoDecimalPart() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, "5.");
        result = testLexer(true, "5.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoIntegerPart() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Float, "-.6");
        result = testLexer(true, "-.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoParts() { return testLexerException<style::LexerException>("-."); }

    test::Result testLexingSemiColon() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::SemiColon);
        result = testLexer(true, ";", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingColon() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Colon);
        result = testLexer(true, ":", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingComma() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Comma);
        result = testLexer(true, ",", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningCurlyBracket() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::OpeningCurlyBracket);
        result = testLexer(true, "{", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingCurlyBracket() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::ClosingCurlyBracket);
        result = testLexer(true, "}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningParenthesis() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::OpeningParenthesis);
        result = testLexer(true, "(", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingParenthesis() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::ClosingParenthesis);
        result = testLexer(true, ")", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingGreatherThan() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::GreaterThan);
        result = testLexer(true, ">", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSharp() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Sharp);
        result = testLexer(true, "#", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingDot() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Dot);
        result = testLexer(true, ".", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingAt() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::At);
        result = testLexer(true, "@", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStar() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::Star);
        result = testLexer(true, "*", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawName() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::RawName, "test");
        result = testLexer(true, "test", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithHyphen() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::RawName, "test-a");
        result = testLexer(true, "test-a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithNumber() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::RawName, "test2");
        result = testLexer(true, "test2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithUnderscore() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::RawName, "test_2");
        result = testLexer(true, "test_2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameStartingWithNumber() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::RawName, "2a");
        result = testLexer(true, "2a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMinusSign() { return testLexerException<style::LexerException>("-"); }

    test::Result testLexingStringDoubleQuotes() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::String, "value");
        result = testLexer(true, "\"value\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringSingleQuotes() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::String, "value");
        result = testLexer(true, "'value'", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringUnclosedDoubleQuotes() { return testLexerException<style::LexerException>("\"value"); }

    test::Result testLexingStringUnclosedSingleQuotes() { return testLexerException<style::LexerException>("'value"); }

    test::Result testLexingStringEmptyDoubleQuotes() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::String, "");
        result = testLexer(true, "\"\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringEmptySingleQuotes() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::String, "");
        result = testLexer(true, "''", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingImport() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::At);
        rootExpected->appendNext(new style::DeserializationNode(style::Token::RawName, "import"))
            ->appendNext(new style::DeserializationNode(style::Token::Space))
            ->appendNext(new style::DeserializationNode(style::Token::String, "test"))
            ->appendNext(new style::DeserializationNode(style::Token::SemiColon));
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
