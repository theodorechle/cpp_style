#include "tests_parser.hpp"

namespace testsParserFileBlock {
    test::Result testParsingEmpty() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        result = testsParser::testLexerAndParser(true, "", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "a"));
        result = testsParser::testLexerAndParser(false, "a {b: #a;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "abcdef"));
        result = testsParser::testLexerAndParser(true, "a {b: #abcdef;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingHexRuleOnlyInts() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "000000"));
        result = testsParser::testLexerAndParser(true, "a {b: #000000;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRule() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        result = testsParser::testLexerAndParser(true, "a {b: 1;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIntRuleMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "123456"));
        result = testsParser::testLexerAndParser(true, "a {b: 123456;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEmptyTuple() {
        return testsParser::testParserError("a {b: ();}", {style::parser::ErrorType::ERROR, "tryParseRuleValue: Empty tuple"});
    }

    test::Result testParsingIntTuple() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "3"));
        result = testsParser::testLexerAndParser(true, "a {b: (1, 2,3);}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingEnumTuple() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Tuple));
        expected->addChild(new style::DeserializationNode(style::Token::EnumValue, "aaa"));
        expected->addChild(new style::DeserializationNode(style::Token::EnumValue, "bbb"));
        expected->addChild(new style::DeserializationNode(style::Token::EnumValue, "ccc"));
        result = testsParser::testLexerAndParser(true, "a {b: (aaa, bbb,ccc);}", rootExpected);
        delete rootExpected;
        return result;
    }

    // TODO: add tests for other data types

    test::Result testParsingRuleNoSemiColon() {
        return testsParser::testParserError("a {b: #aaaaaa}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a semi colon"});
    }

    test::Result testParsingRuleWithoutValue() {
        return testsParser::testParserError("a {b:;}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a rule value"});
    }

    test::Result testParsingRuleWithoutValueAndSemiColon() {
        return testsParser::testParserError("a {b:}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a rule value"});
    }

    test::Result testParsingRuleWithoutColonAndValueAndSemiColon() {
        return testsParser::testParserError("a {b}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a colon"});
    }

    test::Result testParsingRuleWithoutColonAndValue() {
        return testsParser::testParserError("a {b;}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a colon"});
    }

    test::Result testParsingRuleWithoutName() {
        return testsParser::testParserError("a {: #aaaaaa;}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a rule name"});
    }

    test::Result testParsingRuleWithoutNameAndColon() {
        return testsParser::testParserError("a {#aaaaaa;}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a rule name"});
    }

    test::Result testParsingBlockWithoutDeclaration() {
        return testsParser::testParserError("{b: #aaaaaa;}", {style::parser::ErrorType::ERROR, "tryParseSelectorsAndBlock: Invalid selectors block"});
    }

    test::Result testParsingBlockWithoutOpeningCurlyBracket() {
        return testsParser::testParserError("a b: #aaaaaa;}", {style::parser::ErrorType::ERROR, "tryParseSelectorsAndBlock: Invalid rules block"});
    }

    test::Result testParsingBlockWithoutClosingCurlyBracket() {
        return testsParser::testParserError("a {b: #aaaaaa;", {style::parser::ErrorType::ERROR, "tryParseSelectorsAndBlock: Invalid rules block"});
    }

    test::Result testParsingBlockWithoutRuleNameAndValue() {
        return testsParser::testParserError("a {:;}", {style::parser::ErrorType::ERROR, "tryParseRuleAssignment: Missing a rule name"});
    }

    test::Result testParsingBlockWithoutSelectors() {
        return testsParser::testParserError("{b: #aaaaaa;", {style::parser::ErrorType::ERROR, "tryParseSelectorsAndBlock: Invalid selectors block"});
    }

    test::Result testParsingElementNameSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a {}", rootExpected);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifierSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "#a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassSingleChar() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Class, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, ".a {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a > .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "#abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Class, "abc"));
        expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, ".abc {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a #b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a .b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a#b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClassStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a.b {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesInline() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "r"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "j"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a {r: 1; j: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingMultipleRulesMultipleLines() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "r"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "1"));
        expected = expected->parent()->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "j"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a {\nr: 1;\nj: 2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakInBlockDeclaration() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent, ""));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        expected = expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a\nb {}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakAfterBlockDeclaration() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->parent()->parent()->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        result = testsParser::testLexerAndParser(true, "a\n{}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakAfterAssignmentColon() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b:\n2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakBeforeAssignmentColon() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b\n:2;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testParsingLineBreakBeforeSemiColon() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations))
                       ->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "b"));
        expected->addChild(new style::DeserializationNode(style::Token::Int, "2"));
        result = testsParser::testLexerAndParser(true, "a\n{b:2\n;}", rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testTwoStyleBlocks() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
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

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));

        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedModifierBlock() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        std::string text = R"(// equivalent of previous two blocks
label.blue {
    text-color: #0000ff;
    &:hovered {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement, ""));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedElementNameBlock() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        std::string text = R"(// same as test 2 but with an element instead of a modifier in the inner block
label.blue {
    text-color: #0000ff;
    element {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement, ""));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleDefinitionUsingAnyParentRelation() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        std::string text = R"(label.blue:hovered element.red#root {
    text-color: #0000ff;
    element {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleDefinitionUsingAnyChildComponentWithNestedElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        std::string text = R"(label.blue:hovered, element.red#root {
    text-color: #0000ff;
    element {
        text-color: (150,150,150);
    }
})";

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "label"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "blue"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Modifier, "hovered"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "element"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "red"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "root"));
        expected = expected->parent();
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::BlockDeclarations));
        expected = expected->addChild(new style::DeserializationNode(style::Token::Assignment));
        expected->addChild(new style::DeserializationNode(style::Token::RuleName, "text-color"));
        expected->addChild(new style::DeserializationNode(style::Token::Hex, "0000ff"));
        expected = expected->parent();
        expected = expected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testValuesUnits() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        std::string text = R"(element { /* test units */
    width: 150px;
    height: 40%;
})";

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::StyleBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock));
        expected = expected->addChild(new style::DeserializationNode(style::Token::SelectorsList));
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

        result = testsParser::testLexerAndParser(true, text, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testMultilineCommentNotClosed() { return testsParser::testLexerException<style::UnknownValue>("/* not closed comment"); }

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
        tests->addTest(testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart,
                       "Any parent relation identifier sticked to first declaration part");
        tests->addTest(testParsingAnyParentRelationClassStickedToFirstDeclarationPart,
                       "Any parent relation with class sticked to first declaration part");
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

        tests->beginTestBlock("Tests style lexer and parser");
        tests->addTest(testTwoStyleBlocks, "Two style blocks");
        tests->addTest(testNestedModifierBlock, "Nested modifier block");
        tests->addTest(testNestedElementNameBlock, "Nested element name block");
        tests->addTest(testApplyingStyleDefinitionUsingAnyParentRelation, "Apply style block using the any parent relation");
        tests->addTest(testApplyingStyleDefinitionUsingAnyChildComponentWithNestedElementName,
                       "Apply style block to any child component with nested element name");
        tests->addTest(testValuesUnits, "Values units");
        tests->addTest(testMultilineCommentNotClosed, "Multiline comment not closed");
        tests->endTestBlock();

        tests->endTestBlock();
    }
}
