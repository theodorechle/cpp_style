#include "style_tests_lexer_and_parser.hpp"

namespace styleTestsLexerAndParser {
    test::Result testLexer(bool equal, const std::string &expr, const style::DeserializationNode *expected) {
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *result = style::Lexer(&guiStyleConfig, expr).getResult();
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

    test::Result testParser(bool equal, style::DeserializationNode *expr, const style::DeserializationNode *expected) {
        test::Result testResult;
        std::cout << "Test if parsing\n";
        expr->displayNexts(std::cout);
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expr->debugDisplay(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *result = style::Parser(expr).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = test::Result::SUCCESS;
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

    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::DeserializationNode *expected) {
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->debugDisplay(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *tokens = style::Lexer(&guiStyleConfig, expr).getResult();
            style::DeserializationNode *result = style::Parser(tokens).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
            delete tokens;
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
            tokens = style::Lexer(&guiStyleConfig, expression).getResult();
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

    template <typename T>
    test::Result testLexerAndParserException(const std::string &expression) {
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expression << "\n'\n raises an exception : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        style::DeserializationNode *result = nullptr;
        try {
            tokens = style::Lexer(&guiStyleConfig, expression).getResult();
            result = style::Parser(tokens).getFinalTree();
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
        delete result;
        std::cout << "\n";
        return testResult;
    }

    std::string getFileContent(std::string fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
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

    test::Result testParsingEmpty() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        result = testLexerAndParser(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRule() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "a"));
        result = testLexerAndParser(true, "a {b: #a;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "abcdef"));
        result = testLexerAndParser(true, "a {b: #abcdef;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleOnlyInts() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "000000"));
        result = testLexerAndParser(true, "a {b: #000000;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRule() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        result = testLexerAndParser(true, "a {b: 1;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRuleMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "123456"));
        result = testLexerAndParser(true, "a {b: 123456;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEmptyTuple() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        result = testLexerAndParser(true, "a {b: ();}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntTuple() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent))->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "3"));
        result = testLexerAndParser(true, "a {b: (1, 2,3);}", rootExpected);
        delete rootExpected;
        return result;
    }

    // TODO: add tests for other data types

    test::Result testParsingRuleNoSemiColon() { return testLexerAndParserException<style::MissingToken>("a {b: #aaaaaa}"); }

    test::Result testParsingRuleWithoutValue() { return testLexerAndParserException<style::MalformedExpression>("a {b:;}"); }

    test::Result testParsingRuleWithoutValueAndSemiColon() { return testLexerAndParserException<style::MissingToken>("a {b:}"); }

    test::Result testParsingRuleWithoutColonAndValueAndSemiColon() { return testLexerAndParserException<style::MalformedExpression>("a {b}"); }

    test::Result testParsingRuleWithoutColonAndValue() { return testLexerAndParserException<style::MalformedExpression>("a {b;}"); }

    test::Result testParsingRuleWithoutName() { return testLexerAndParserException<style::MalformedExpression>("a {: #aaaaaa;}"); }

    test::Result testParsingRuleWithoutNameAndColon() { return testLexerAndParserException<style::MalformedExpression>("a {#aaaaaa;}"); }

    test::Result testParsingBlockWithoutDeclaration() { return testLexerAndParserException<style::MalformedExpression>("{b: #aaaaaa;}"); }

    test::Result testParsingBlockWithoutOpeningCurlyBracket() { return testLexerAndParserException<style::MalformedExpression>("a b: #aaaaaa;}"); }

    test::Result testParsingBlockWithoutClosingCurlyBracket() { return testLexerAndParserException<style::MalformedExpression>("a {b: #aaaaaa;"); }

    test::Result testParsingElementNameSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a {}", rootExpected);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifierSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "#a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::Class, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, ".a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a > b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a > #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a > .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "#abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::Class, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, ".abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a#b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClassStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockContent));
        result = testLexerAndParser(true, "a.b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesInline() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "r"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "j"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testLexerAndParser(true, "a {r: 1; j: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesMultipleLines() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration))
            ->addChild(new style::DeserializationNode(style::Token::Declaration))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "r"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "j"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testLexerAndParser(true, "a {\nr: 1;\nj: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakInBlockDeclaration() { return testLexerAndParserException<style::MalformedExpression>("a\nb {}"); }

    test::Result testParsingLineBreakAfterBlockDeclaration() { return testLexerAndParserException<style::MalformedExpression>("a\n{}"); }

    test::Result testParsingLineBreakAfterAssignmentColon() { return testLexerAndParserException<style::MalformedExpression>("a\n{b:\n2;}"); }

    test::Result testParsingLineBreakBeforeAssignmentColon() { return testLexerAndParserException<style::MalformedExpression>("a\n{b\n:2;}"); }

    test::Result testParsingLineBreakBeforeSemiColon() { return testLexerAndParserException<style::MalformedExpression>("a\n{b:2\n;}"); }

    test::Result testTwoStyleBlocks() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-1.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));

        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "255"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "112"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedModifierBlock() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-2.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedElementNameBlock() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-3.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyParentRelation() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-4.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-5.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testValuesUnits() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-7.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclaration));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Declaration));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockContent));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "width"));
        expected->addChild(new style::DeserializationNode(style::Token::Unit, "px"))->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "height"));
        expected->addChild(new style::DeserializationNode(style::Token::Unit, "%"))->addChild(new style::DeserializationNode(style::Token::Int, "40"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testMultilineCommentNotClosed() {
        std::string fileContent;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-9.txt");
        return testLexerAndParserException<style::UnknownValue>(fileContent);
    }

    void testsLexerAndParser(test::Tests *tests) {
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

        tests->beginTestBlock("Pseudo names");
        tests->addTest(testLexingRawName, "Pseudo name");
        tests->addTest(testLexingRawNameWithHyphen, "Pseudo name with hyphen");
        tests->addTest(testLexingRawNameWithNumber, "Pseudo name with number");
        tests->addTest(testLexingRawNameWithUnderscore, "Pseudo name with underscore");
        tests->addTest(testLexingRawNameStartingWithNumber, "Pseudo name with starting with number");
        tests->addTest(testLexingMinusSign, "Minus sign");
        tests->endTestBlock();

        tests->beginTestBlock("Pseudo names");
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

        tests->beginTestBlock("Tests parser");
        tests->addTest(testParsingEmpty, "Empty");

        tests->beginTestBlock("Data types");
        tests->addTest(testParsingHexRule, "Hex rule");
        tests->addTest(testParsingHexRuleMultipleChars, "Hex rule multiple chars");
        tests->addTest(testParsingHexRuleOnlyInts, "Hex rule only ints");
        tests->addTest(testParsingIntRule, "Int rule");
        tests->addTest(testParsingIntRuleMultipleChars, "Int rule multiple chars");
        tests->addTest(testParsingEmptyTuple, "Empty tuple");
        tests->addTest(testParsingIntTuple, "Int tuple");
        tests->endTestBlock();

        tests->beginTestBlock("Invalid blocks structures");
        tests->addTest(testParsingRuleNoSemiColon, "Rule no semi-colon");
        tests->addTest(testParsingRuleWithoutValue, "Rule without value");
        tests->addTest(testParsingRuleWithoutValueAndSemiColon, "Rule without value and semi-colon");
        tests->addTest(testParsingRuleWithoutColonAndValueAndSemiColon, "Rule without colon and value and semi-colon");
        tests->addTest(testParsingRuleWithoutColonAndValue, "Rule without rule colon and value");
        tests->addTest(testParsingRuleWithoutName, "Rule without name");
        tests->addTest(testParsingRuleWithoutNameAndColon, "Rule without name and colon");
        tests->addTest(testParsingBlockWithoutDeclaration, "Block without declaration");
        tests->addTest(testParsingBlockWithoutOpeningCurlyBracket, "Block without opening curly bracket");
        tests->addTest(testParsingBlockWithoutClosingCurlyBracket, "Block without closing curly bracket");
        tests->endTestBlock();

        tests->beginTestBlock("Declaration parts");
        tests->addTest(testParsingElementNameSingleChar, "Element name single char");
        tests->addTest(testParsingIdentifierSingleChar, "Identifier single char");
        tests->addTest(testParsingClassSingleChar, "Class single char");
        tests->addTest(testParsingElementNameMultipleChars, "Element name multiple chars");
        tests->addTest(testParsingIdentifierMultipleChars, "Identfier multiple chars");
        tests->addTest(testParsingClassMultipleChars, "Class multiple chars");

        tests->beginTestBlock("Declaration parts relations");
        tests->addTest(testParsingDirectParentRelationElementName, "Direct parent relation with element name");
        tests->addTest(testParsingDirectParentRelationIdentifier, "Direct parent relation with identifier");
        tests->addTest(testParsingDirectParentRelationClass, "Direct parent relation with class");
        tests->addTest(testParsingAnyParentRelationElementName, "Any parent relation with element name");
        tests->addTest(testParsingAnyParentRelationIdentifier, "Any parent relation with identifier");
        tests->addTest(testParsingAnyParentRelationClass, "Any parent relation with class");
        tests->addTest(testParsingAnyParentRelationIdentifier, "Any parent relation with identifier sticked to first declaration part");
        tests->addTest(testParsingAnyParentRelationClass, "Any parent relation with class sticked to first declaration part");
        tests->endTestBlock();
        tests->endTestBlock();

        tests->beginTestBlock("Whitespaces");
        tests->addTest(testParsingMultipleRulesInline, "Multiple rules inline");
        tests->addTest(testParsingMultipleRulesMultipleLines, "Multiple rules multiple lines");
        tests->addTest(testParsingLineBreakInBlockDeclaration, "Line break in block declaration");
        tests->addTest(testParsingLineBreakAfterBlockDeclaration, "Line break after block declaration");
        tests->addTest(testParsingLineBreakAfterAssignmentColon, "Line break after assignment colon");
        tests->addTest(testParsingLineBreakBeforeAssignmentColon, "Line break before assignment colon");
        tests->addTest(testParsingLineBreakBeforeSemiColon, "Line break before semi-colon");
        tests->endTestBlock();
        tests->endTestBlock();

        tests->beginTestBlock("Tests style lexer and parser");
        tests->addTest(testTwoStyleBlocks, "Two style blocks");
        tests->addTest(testNestedModifierBlock, "Nested modifier block");
        tests->addTest(testNestedElementNameBlock, "Nested element name block");
        tests->addTest(testApplyingStyleBlockUsingAnyParentRelation, "Apply style block using the any parent relation");
        tests->addTest(testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName,
                       "Apply style block to any child component with nested element name");
        tests->addTest(testValuesUnits, "Values units");
        tests->addTest(testMultilineCommentNotClosed, "Multiline comment not closed");
        tests->endTestBlock();
    }

} // namespace styleTestsLexerAndParser
