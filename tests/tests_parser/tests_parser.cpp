#include "tests_parser.hpp"

namespace testsParser {

    test::Result testLexerAndParser(bool equal, const std::string &expression, const style::DeserializationNode *expected) {
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expression << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->debugDisplay(std::cout);
        std::cout << ":\n";
        try {
            style::DeserializationNode *tokens = style::Lexer().lexe(expression);
            style::DeserializationNode *result = style::Parser().parse(tokens);
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
    test::Result testLexerAndParserException(const std::string &expression) {
        test::Result testResult;
        std::cout << "Test if lexing and parsing\n'\n" << expression << "\n'\n raises an exception : ";
#ifdef DEBUG
        std::cout << "\n";
#endif
        style::DeserializationNode *tokens = nullptr;
        style::DeserializationNode *result = nullptr;
        try {
            tokens = style::Lexer().lexe(expression);
            result = style::Parser().parse(tokens);
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
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

    test::Result testParsingRuleNoSemiColon() { return testLexerAndParserException<style::MissingTokenException>("a {b: #aaaaaa}"); }

    test::Result testParsingRuleWithoutValue() { return testLexerAndParserException<style::MalformedExpressionException>("a {b:;}"); }

    test::Result testParsingRuleWithoutValueAndSemiColon() { return testLexerAndParserException<style::MissingTokenException>("a {b:}"); }

    test::Result testParsingRuleWithoutColonAndValueAndSemiColon() { return testLexerAndParserException<style::MalformedExpressionException>("a {b}"); }

    test::Result testParsingRuleWithoutColonAndValue() { return testLexerAndParserException<style::MalformedExpressionException>("a {b;}"); }

    test::Result testParsingRuleWithoutName() { return testLexerAndParserException<style::MalformedExpressionException>("a {: #aaaaaa;}"); }

    test::Result testParsingRuleWithoutNameAndColon() { return testLexerAndParserException<style::MalformedExpressionException>("a {#aaaaaa;}"); }

    test::Result testParsingBlockWithoutDeclaration() { return testLexerAndParserException<style::MalformedExpressionException>("{b: #aaaaaa;}"); }

    test::Result testParsingBlockWithoutOpeningCurlyBracket() { return testLexerAndParserException<style::MalformedExpressionException>("a b: #aaaaaa;}"); }

    test::Result testParsingBlockWithoutClosingCurlyBracket() { return testLexerAndParserException<style::MalformedExpressionException>("a {b: #aaaaaa;"); }

    test::Result testParsingElementNameSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::Class, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::Class, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
                       ->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors))
            ->addChild(new style::DeserializationNode(style::Token::Selector))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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

    test::Result testParsingLineBreakInBlockDeclaration() { return testLexerAndParserException<style::MalformedExpressionException>("a\nb {}"); }

    test::Result testParsingLineBreakAfterBlockDeclaration() { return testLexerAndParserException<style::MalformedExpressionException>("a\n{}"); }

    test::Result testParsingLineBreakAfterAssignmentColon() { return testLexerAndParserException<style::MalformedExpressionException>("a\n{b:\n2;}"); }

    test::Result testParsingLineBreakBeforeAssignmentColon() { return testLexerAndParserException<style::MalformedExpressionException>("a\n{b\n:2;}"); }

    test::Result testParsingLineBreakBeforeSemiColon() { return testLexerAndParserException<style::MalformedExpressionException>("a\n{b:2\n;}"); }

    test::Result testTwoStyleBlocks() {
        std::string fileContent;
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-1.txt");

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));

        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
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
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockSelectors));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Selector));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "width"));
        expected->addChild(new style::DeserializationNode(style::Token::Unit, "px"))
            ->addChild(new style::DeserializationNode(style::Token::Int, "150"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "height"));
        expected->addChild(new style::DeserializationNode(style::Token::Unit, "%"))
            ->addChild(new style::DeserializationNode(style::Token::Int, "40"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testMultilineCommentNotClosed() {
        std::string fileContent;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-9.txt");
        return testLexerAndParserException<style::UnknownValue>(fileContent);
    }

    void testsParser(test::Tests *tests) {

        tests->beginTestBlock("Tests parser");
        tests->addTest(testParsingEmpty, "Empty");

        tests->beginTestBlock("Data types", false);
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

        tests->beginTestBlock("Selectors");
        tests->addTest(testParsingElementNameSingleChar, "Element name single char");
        tests->addTest(testParsingIdentifierSingleChar, "Identifier single char");
        tests->addTest(testParsingClassSingleChar, "Class single char");
        tests->addTest(testParsingElementNameMultipleChars, "Element name multiple chars");
        tests->addTest(testParsingIdentifierMultipleChars, "Identfier multiple chars");
        tests->addTest(testParsingClassMultipleChars, "Class multiple chars");

        tests->beginTestBlock("Selectors relations");
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

} // namespace testsParser
