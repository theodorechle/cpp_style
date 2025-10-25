#include "style_tests_lexer_and_parser.hpp"

namespace styleTestsLexerAndParser {
    test::Result testLexer(bool equal, const std::string &expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if lexing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ":\n";
        try {
            style::Node *result = style::Lexer(expr).getResult();
            style::Node *n = result;
            std::cout << "result:\n";
            n->displayNexts();
            while (n != nullptr) {
                if ((expected == nullptr || !(*n == *expected)) == equal) {
                    delete result;
                    return test::Result::FAILURE;
                }
                n = n->getNext();
                expected = expected->getNext();
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

    test::Result testParser(bool equal, style::Node *expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if parsing\n";
        expr->displayNexts(std::cout);
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expr->display(std::cout);
        std::cout << ":\n";
        try {
            style::Node *result = style::Parser(expr).getFinalTree();
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

    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->display(std::cout);
        std::cout << ":\n";
        try {
            style::Node *tokens = style::Lexer(expr).getResult();
            style::Node *result = style::Parser(tokens).getFinalTree();
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
        style::Node *tokens = nullptr;
        try {
            tokens = style::Lexer(expression).getResult();
#ifdef DEBUG
            tokens->display();
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
        style::Node *tokens = nullptr;
        style::Node *result = nullptr;
        try {
            tokens = style::Lexer(expression).getResult();
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
        style::Node *rootExpected;
        test::Result result;

        rootExpected = nullptr;
        result = testLexer(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleSpace() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Space);
        result = testLexer(true, " ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleSpaces() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Space);
        result = testLexer(true, "   ", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingTabulation() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Space);
        result = testLexer(true, "\t", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSingleLineBreak() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::LineBreak);
        result = testLexer(true, "\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMultipleLineBreaks() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::LineBreak);
        result = testLexer(true, "\n\n\n\n", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveInteger() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Int, "1");
        result = testLexer(true, "1", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerZero() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Int, "0");
        result = testLexer(true, "0", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeInteger() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Int, "-5");
        result = testLexer(true, "-5", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingIntegerWithMultipleChars() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Int, "2679");
        result = testLexer(true, "2679", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingPositiveFloat() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, "26.3");
        result = testLexer(true, "26.3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatZero() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, "0.");
        result = testLexer(true, "0.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloat() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, "-12.6");
        result = testLexer(true, "-12.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoIntegralPart() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, ".3");
        result = testLexer(true, ".3", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingFloatNoDecimalPart() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, "5.");
        result = testLexer(true, "5.", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoIntegerPart() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Float, "-.6");
        result = testLexer(true, "-.6", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingNegativeFloatNoParts() { return testLexerException<style::LexerException>("-."); }

    test::Result testLexingSemiColon() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::SemiColon);
        result = testLexer(true, ";", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingColon() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Colon);
        result = testLexer(true, ":", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingComma() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Comma);
        result = testLexer(true, ",", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningCurlyBracket() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::OpeningCurlyBracket);
        result = testLexer(true, "{", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingCurlyBracket() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::ClosingCurlyBracket);
        result = testLexer(true, "}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingOpeningParenthesis() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::OpeningParenthesis);
        result = testLexer(true, "(", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingClosingParenthesis() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::ClosingParenthesis);
        result = testLexer(true, ")", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingGreatherThan() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::GreaterThan);
        result = testLexer(true, ">", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingSharp() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Sharp);
        result = testLexer(true, "#", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingDot() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Dot);
        result = testLexer(true, ".", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingAt() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::At);
        result = testLexer(true, "@", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStar() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::Star);
        result = testLexer(true, "*", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawName() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::RawName, "test");
        result = testLexer(true, "test", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithHyphen() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::RawName, "test-a");
        result = testLexer(true, "test-a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithNumber() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::RawName, "test2");
        result = testLexer(true, "test2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameWithUnderscore() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::RawName, "test_2");
        result = testLexer(true, "test_2", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingRawNameStartingWithNumber() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::RawName, "2a");
        result = testLexer(true, "2a", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingMinusSign() { return testLexerException<style::LexerException>("-"); }

    test::Result testLexingStringDoubleQuotes() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::String, "value");
        result = testLexer(true, "\"value\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringSingleQuotes() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::String, "value");
        result = testLexer(true, "'value'", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringUnclosedDoubleQuotes() { return testLexerException<style::LexerException>("\"value"); }

    test::Result testLexingStringUnclosedSingleQuotes() { return testLexerException<style::LexerException>("'value"); }

    test::Result testLexingStringEmptyDoubleQuotes() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::String, "");
        result = testLexer(true, "\"\"", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingStringEmptySingleQuotes() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::String, "");
        result = testLexer(true, "''", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testLexingImport() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::At);
        rootExpected->appendNext(new style::Node(style::Token::RawName, "import"))
            ->appendNext(new style::Node(style::Token::Space))
            ->appendNext(new style::Node(style::Token::String, "test"))
            ->appendNext(new style::Node(style::Token::SemiColon));
        result = testLexer(true, "@import \"test\";", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEmpty() {
        style::Node *rootExpected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        result = testLexerAndParser(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRule() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Hex, "a"));
        result = testLexerAndParser(true, "a {b: #a;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleMultipleChars() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Hex, "abcdef"));
        result = testLexerAndParser(true, "a {b: #abcdef;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleOnlyInts() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Hex, "000000"));
        result = testLexerAndParser(true, "a {b: #000000;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRule() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Int, "1"));
        result = testLexerAndParser(true, "a {b: 1;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRuleMultipleChars() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Int, "123456"));
        result = testLexerAndParser(true, "a {b: 123456;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEmptyTuple() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected->appendChild(new style::Node(style::Token::Tuple));
        result = testLexerAndParser(true, "a {b: ();}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntTuple() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition))->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "b"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "1"));
        expected->appendChild(new style::Node(style::Token::Int, "2"));
        expected->appendChild(new style::Node(style::Token::Int, "3"));
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

    test::Result testParsingElementName() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a {}", rootExpected);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifier() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::Identifier, "a"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "#a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClass() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::Class, "a"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, ".a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::DirectParent));
        expected->appendChild(new style::Node(style::Token::ElementName, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a > b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::DirectParent));
        expected->appendChild(new style::Node(style::Token::Identifier, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a > #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::DirectParent));
        expected->appendChild(new style::Node(style::Token::Class, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a > .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "abc"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::Identifier, "abc"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "#abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::Class, "abc"));
        expected->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, ".abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationElementName() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::ElementName, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifier() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::Identifier, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClass() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::Class, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::Identifier, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a#b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClassStickedToFirstDeclarationPart() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration))->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::Class, "b"));
        expected->getParent()->getParent()->appendChild(new style::Node(style::Token::BlockDefinition));
        result = testLexerAndParser(true, "a.b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesInline() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "r"));
        expected->appendChild(new style::Node(style::Token::Int, "1"));
        expected = expected->getParent()->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "j"));
        expected->appendChild(new style::Node(style::Token::Int, "2"));
        result = testLexerAndParser(true, "a {r: 1; j: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesMultipleLines() {
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected->appendChild(new style::Node(style::Token::BlockDeclaration))
            ->appendChild(new style::Node(style::Token::Declaration))
            ->appendChild(new style::Node(style::Token::ElementName, "a"));
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "r"));
        expected->appendChild(new style::Node(style::Token::Int, "1"));
        expected = expected->getParent()->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "j"));
        expected->appendChild(new style::Node(style::Token::Int, "2"));
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
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-1.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));

        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "255"));
        expected->appendChild(new style::Node(style::Token::Int, "112"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedModifierBlock() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-2.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedElementNameBlock() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-3.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyParentRelation() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-4.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected->appendChild(new style::Node(style::Token::Class, "red"));
        expected->appendChild(new style::Node(style::Token::Identifier, "root"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-5.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected->appendChild(new style::Node(style::Token::Class, "red"));
        expected->appendChild(new style::Node(style::Token::Identifier, "root"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testValuesUnits() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-7.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "width"));
        expected->appendChild(new style::Node(style::Token::PixelUnit))->appendChild(new style::Node(style::Token::Int, "150"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::RuleName, "height"));
        expected->appendChild(new style::Node(style::Token::PercentageUnit))->appendChild(new style::Node(style::Token::Int, "40"));

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
        tests->beginTestBlock("Tests style lexer and parser");
        tests->beginTestBlock("Tests lexer");
        tests->beginTestBlock("White spaces");
        tests->runTest(testLexingEmpty, "Empty");
        tests->runTest(testLexingSingleSpace, "Single space");
        tests->runTest(testLexingMultipleSpaces, "Multiple spaces");
        tests->runTest(testLexingSingleLineBreak, "Single line break");
        tests->runTest(testLexingMultipleLineBreaks, "Multiple line breaks");
        tests->runTest(testLexingTabulation, "Tabulation");
        tests->endTestBlock();

        tests->beginTestBlock("Integers");
        tests->runTest(testLexingPositiveInteger, "Positive integer");
        tests->runTest(testLexingIntegerZero, "Integer 0");
        tests->runTest(testLexingNegativeInteger, "Negative integer");
        tests->runTest(testLexingIntegerWithMultipleChars, "Integer with multiple chars");
        tests->endTestBlock();

        tests->beginTestBlock("Floats");
        tests->runTest(testLexingPositiveFloat, "Positive float");
        tests->runTest(testLexingFloatZero, "Float 0");
        tests->runTest(testLexingNegativeFloat, "Negative float");
        tests->runTest(testLexingFloatNoIntegralPart, "Float no integral part");
        tests->runTest(testLexingFloatNoDecimalPart, "Float no decimal part");
        tests->runTest(testLexingNegativeFloatNoIntegerPart, "Negative float no decimal part");
        tests->runTest(testLexingNegativeFloatNoParts, "Negative float no parts");
        tests->endTestBlock();

        tests->beginTestBlock("Reserved characters");
        tests->runTest(testLexingSemiColon, "Semicolon");
        tests->runTest(testLexingColon, "Colon");
        tests->runTest(testLexingComma, "Comma");
        tests->runTest(testLexingOpeningCurlyBracket, "Opening curly bracket");
        tests->runTest(testLexingClosingCurlyBracket, "Closing curly bracket");
        tests->runTest(testLexingGreatherThan, "Greater than");
        tests->runTest(testLexingSharp, "Sharp");
        tests->runTest(testLexingDot, "Dot");
        tests->runTest(testLexingAt, "At");
        tests->runTest(testLexingStar, "Star");
        tests->endTestBlock();

        tests->beginTestBlock("Pseudo names");
        tests->runTest(testLexingRawName, "Pseudo name");
        tests->runTest(testLexingRawNameWithHyphen, "Pseudo name with hyphen");
        tests->runTest(testLexingRawNameWithNumber, "Pseudo name with number");
        tests->runTest(testLexingRawNameWithUnderscore, "Pseudo name with underscore");
        tests->runTest(testLexingRawNameStartingWithNumber, "Pseudo name with starting with number");
        tests->runTest(testLexingMinusSign, "Minus sign");
        tests->endTestBlock();

        tests->beginTestBlock("Pseudo names");
        tests->runTest(testLexingStringDoubleQuotes, "String double quotes");
        tests->runTest(testLexingStringSingleQuotes, "String single quotes");
        tests->runTest(testLexingStringUnclosedDoubleQuotes, "String unclosed double quotes");
        tests->runTest(testLexingStringUnclosedSingleQuotes, "String unclosed single quotes");
        tests->runTest(testLexingStringEmptyDoubleQuotes, "String empty double quotes");
        tests->runTest(testLexingStringEmptySingleQuotes, "String empty single quotes");
        tests->endTestBlock();

        tests->beginTestBlock("Multiple tokens");
        tests->runTest(testLexingImport, "Import");
        tests->endTestBlock();

        tests->endTestBlock();

        tests->beginTestBlock("Tests parser");
        tests->runTest(testParsingEmpty, "Empty");

        tests->beginTestBlock("Data types");
        tests->runTest(testParsingHexRule, "Hex rule");
        tests->runTest(testParsingHexRuleMultipleChars, "Hex rule multiple chars");
        tests->runTest(testParsingHexRuleOnlyInts, "Hex rule only ints");
        tests->runTest(testParsingIntRule, "Int rule");
        tests->runTest(testParsingIntRuleMultipleChars, "Int rule multiple chars");
        tests->runTest(testParsingEmptyTuple, "Empty tuple");
        tests->runTest(testParsingIntTuple, "Int tuple");
        tests->endTestBlock();

        tests->beginTestBlock("Invalid blocks structures");
        tests->runTest(testParsingRuleNoSemiColon, "Rule no semi-colon");
        tests->runTest(testParsingRuleWithoutValue, "Rule without value");
        tests->runTest(testParsingRuleWithoutValueAndSemiColon, "Rule without value and semi-colon");
        tests->runTest(testParsingRuleWithoutColonAndValueAndSemiColon, "Rule without colon and value and semi-colon");
        tests->runTest(testParsingRuleWithoutColonAndValue, "Rule without rule colon and value");
        tests->runTest(testParsingRuleWithoutName, "Rule without name");
        tests->runTest(testParsingRuleWithoutNameAndColon, "Rule without name and colon");
        tests->runTest(testParsingBlockWithoutDeclaration, "Block without declaration");
        tests->runTest(testParsingBlockWithoutOpeningCurlyBracket, "Block without opening curly bracket");
        tests->runTest(testParsingBlockWithoutClosingCurlyBracket, "Block without closing curly bracket");
        tests->endTestBlock();

        tests->beginTestBlock("Declaration parts");
        tests->runTest(testParsingElementName, "Element name");
        tests->runTest(testParsingIdentifier, "Identifier");
        tests->runTest(testParsingClass, "Class");
        tests->runTest(testParsingElementNameMultipleChars, "Element name multiple chars");
        tests->runTest(testParsingIdentifierMultipleChars, "Identfier multiple chars");
        tests->runTest(testParsingClassMultipleChars, "Class multiple chars");

        tests->beginTestBlock("Declaration parts relations");
        tests->runTest(testParsingDirectParentRelationElementName, "Direct parent relation with element name");
        tests->runTest(testParsingDirectParentRelationIdentifier, "Direct parent relation with identifier");
        tests->runTest(testParsingDirectParentRelationClass, "Direct parent relation with class");
        tests->runTest(testParsingAnyParentRelationElementName, "Any parent relation with element name");
        tests->runTest(testParsingAnyParentRelationIdentifier, "Any parent relation with identifier");
        tests->runTest(testParsingAnyParentRelationClass, "Any parent relation with class");
        tests->runTest(testParsingAnyParentRelationIdentifier, "Any parent relation with identifier sticked to first declaration part");
        tests->runTest(testParsingAnyParentRelationClass, "Any parent relation with class sticked to first declaration part");
        tests->endTestBlock();
        tests->endTestBlock();

        tests->beginTestBlock("Whitespaces");
        tests->runTest(testParsingMultipleRulesInline, "Multiple rules inline");
        tests->runTest(testParsingMultipleRulesMultipleLines, "Multiple rules multiple lines");
        tests->runTest(testParsingLineBreakInBlockDeclaration, "Line break in block declaration");
        tests->runTest(testParsingLineBreakAfterBlockDeclaration, "Line break after block declaration");
        tests->runTest(testParsingLineBreakAfterAssignmentColon, "Line break after assignment colon");
        tests->runTest(testParsingLineBreakBeforeAssignmentColon, "Line break before assignment colon");
        tests->runTest(testParsingLineBreakBeforeSemiColon, "Line break before semi-colon");
        tests->endTestBlock();

        tests->runTest(testTwoStyleBlocks, "Two style blocks");
        tests->runTest(testNestedModifierBlock, "Nested modifier block");
        tests->runTest(testNestedElementNameBlock, "Nested element name block");
        tests->runTest(testApplyingStyleBlockUsingAnyParentRelation, "Apply style block using the any parent relation");
        tests->runTest(testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName,
                       "Apply style block to any child component with nested element name");
        tests->runTest(testValuesUnits, "Values units");
        tests->runTest(testMultilineCommentNotClosed, "Multiline comment not closed");

        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace styleTestsLexerAndParser
