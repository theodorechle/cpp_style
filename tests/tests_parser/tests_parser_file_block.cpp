#include "tests_parser.hpp"

namespace testsParserFileBlock {
    test::Result testParsingEmpty() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        result = testsParser::testLexerAndParser(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleSingleChar() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "a"));
        result = testsParser::testLexerAndParser(false, "a {b: #a;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleMultipleChars() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "abcdef"));
        result = testsParser::testLexerAndParser(true, "a {b: #abcdef;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleOnlyInts() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "000000"));
        result = testsParser::testLexerAndParser(true, "a {b: #000000;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRule() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "1"));
        result = testsParser::testLexerAndParser(true, "a {b: 1;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRuleMultipleChars() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "123456"));
        result = testsParser::testLexerAndParser(true, "a {b: 123456;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEmptyTuple() { return testsParser::testParserError("a {b: ();}", {style::parser::ErrorType::ERROR, "Empty tuple"}); }

    test::Result testParsingIntTuple() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "1"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "3"));
        result = testsParser::testLexerAndParser(true, "a {b: (1, 2,3);}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEnumTuple() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::EnumValue, "aaa"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::EnumValue, "bbb"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::EnumValue, "ccc"));
        result = testsParser::testLexerAndParser(true, "a {b: (aaa, bbb,ccc);}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingString() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "font-name"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::String, "a-font"));
        result = testsParser::testLexerAndParser(true, "a {font-name: 'a-font';}", rootExpected);
        delete rootExpected;
        return result;
    }

    // TODO: add tests for other data types

    test::Result testParsingRuleNoSemiColon() {
        return testsParser::testParserError("a {b: #aaaaaa}", {style::parser::ErrorType::ERROR, "Missing a semi colon after rule assignment"});
    }

    test::Result testParsingRuleWithoutValue() {
        return testsParser::testParserError("a {b:;}", {style::parser::ErrorType::ERROR, "Missing a rule value in rule assignment"});
    }

    test::Result testParsingRuleWithoutValueAndSemiColon() {
        return testsParser::testParserError("a {b:}", {style::parser::ErrorType::ERROR, "Missing a rule value in rule assignment"});
    }

    test::Result testParsingRuleWithoutColonAndValueAndSemiColon() {
        return testsParser::testParserError("a {b}", {style::parser::ErrorType::ERROR, "Missing a colon in rule assignment"});
    }

    test::Result testParsingRuleWithoutColonAndValue() {
        return testsParser::testParserError("a {b;}", {style::parser::ErrorType::ERROR, "Missing a colon in rule assignment"});
    }

    test::Result testParsingRuleWithoutName() {
        return testsParser::testParserError("a {: #aaaaaa;}", {style::parser::ErrorType::LOG, "Not a rule assignment"});
    }

    test::Result testParsingRuleWithoutNameAndColon() {
        return testsParser::testParserError("a {#aaaaaa;}", {style::parser::ErrorType::LOG, "Not a rule assignment"});
    }

    test::Result testParsingBlockWithoutDeclaration() {
        return testsParser::testParserError("{b: #aaaaaa;}", {style::parser::ErrorType::LOG, "Not a valid selectors block"});
    }

    test::Result testParsingBlockWithoutOpeningCurlyBracket() {
        return testsParser::testParserError("a b: #aaaaaa;}", {style::parser::ErrorType::ERROR, "Invalid rules block"});
    }

    test::Result testParsingBlockWithoutClosingCurlyBracket() {
        return testsParser::testParserError("a {b: #aaaaaa;", {style::parser::ErrorType::ERROR, "Invalid rules block"});
    }

    test::Result testParsingBlockWithoutRuleNameAndValue() {
        return testsParser::testParserError("a {:;}", {style::parser::ErrorType::LOG, "Not a rule assignment"});
    }

    test::Result testParsingBlockWithoutSelectors() {
        return testsParser::testParserError("{b: #aaaaaa;}", {style::parser::ErrorType::LOG, "No selectors found"});
    }

    test::Result testParsingBlockWithInvalidSelectors() {
        return testsParser::testParserError(">& {b: #aaaaaa;}", {style::parser::ErrorType::LOG, "Not a valid selectors block"});
    }

    test::Result testParsingBlockWithoutRulesBlock() {
        return testsParser::testParserError("a > .b", {style::parser::ErrorType::ERROR, "Missing a rules block"});
    }

    test::Result testParsingElementNameSingleChar() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a {}", rootExpected);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifierSingleChar() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "#a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassSingleChar() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Class, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, ".a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "abc"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "abc"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "#abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Class, "abc"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, ".abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesInline() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "r"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "j"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a {r: 1; j: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesMultipleLines() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "r"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "j"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a {\nr: 1;\nj: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakInBlockDeclaration() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::AnyParent, ""));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "b"));
        expected = expected->parent()->parent()->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a\nb {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakAfterBlockDeclaration() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->parent()->parent()->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a\n{}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakAfterAssignmentColon() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b:\n2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakBeforeAssignmentColon() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b\n:2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakBeforeSemiColon() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "b"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b:2\n;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testTwoStyleBlocks() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        std::string text = R"(label.blue { // labels with the class 'blue' will have a text in blue
    text-color: #0000ff;
}

label.blue:hovered {
    /*
    labels with the class 'blue' will have a text in gray
    this override the previous declaration block of 'label.blue' when it's hovered
    */
    text-color: (150, 255, 112);
})";

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "label"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "0000ff"));

        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "label"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "blue"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "255"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "112"));

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedModifierBlock() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        std::string text = R"(// equivalent of previous two blocks
label.blue {
    text-color: #0000ff;
    &:hovered {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "label"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement, ""));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedElementNameBlock() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        std::string text = R"(// same as test 2 but with an element instead of a modifier in the inner block
label.blue {
    text-color: #0000ff;
    element {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "label"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement, ""));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedBlockWithDirectParentSelector() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        std::string text = R"(// same as test 2 but with an element instead of a modifier in the inner block
label.blue {
    text-color: #0000ff;
    >element {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "label"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement, ""));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "text-color"));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Tuple));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testValuesUnits() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        std::string text = R"(element { /* test units */
    width: 150px;
    height: 40%;
})";

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::StyleBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "element"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::BlockDeclarations));
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "width"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Unit, "px"))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Int, "150"));
        expected = expected->parent();
        expected = expected->addChild(new style::parser::ParserNode(style::parser::Token::Assignment));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::RuleName, "height"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Unit, "%"))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Int, "40"));

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testMultilineCommentNotClosed() { return testsParser::testLexerException<style::lexer::UnknownValue>("/* not closed comment"); }

    test::Result testImport() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::Import, "file_to_import.style"));

        result = testsParser::testLexerAndParser(true, "@import 'file_to_import.style';", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testImportWithoutSemiColon() {
        return testsParser::testParserError("@import 'file_to_import.style'", {style::parser::ErrorType::ERROR, "Missing a semi-colon after import"});
    }

    void testsParsingFileBlock(test::Tests *tests) {
        tests->beginTestBlock("Parsing file block");
        tests->addTest(testParsingEmpty, "Empty");

        tests->beginTestBlock("Data types");
        tests->addTest(testParsingHexRuleSingleChar, "Hex rule");
        tests->addTest(testParsingHexRuleMultipleChars, "Hex rule multiple chars");
        tests->addTest(testParsingHexRuleOnlyInts, "Hex rule only ints");
        tests->addTest(testParsingIntRule, "Int rule");
        tests->addTest(testParsingIntRuleMultipleChars, "Int rule multiple chars");
        tests->addTest(testParsingEmptyTuple, "Empty tuple");
        tests->addTest(testParsingIntTuple, "Int tuple");
        tests->addTest(testParsingEnumTuple, "Enum tuple");
        tests->addTest(testParsingString, "String");
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
        tests->addTest(testParsingBlockWithoutRuleNameAndValue, "Block without rule name and value");
        tests->addTest(testParsingBlockWithoutSelectors, "Block without selectors");
        tests->addTest(testParsingBlockWithInvalidSelectors, "Block with invalid selectors");
        tests->addTest(testParsingBlockWithoutRulesBlock, "Block without rules block");
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

        tests->beginTestBlock("Style lexer and parser");
        tests->addTest(testTwoStyleBlocks, "Two style blocks");
        tests->addTest(testNestedModifierBlock, "Nested modifier block");
        tests->addTest(testNestedElementNameBlock, "Nested element name block");
        tests->addTest(testNestedBlockWithDirectParentSelector, "Nested block with direct parent selector");
        tests->addTest(testValuesUnits, "Values units");
        tests->addTest(testMultilineCommentNotClosed, "Multiline comment not closed");
        tests->endTestBlock();

        tests->beginTestBlock("At rules");
        tests->addTest(testImport, "Import");
        tests->addTest(testImportWithoutSemiColon, "Import without semi-colon");
        tests->endTestBlock();

        tests->endTestBlock();
    }
}
