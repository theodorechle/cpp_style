#include "tests_parser.hpp"
namespace testsParserSelectorsBlock {
    test::Result testParsingElementNameSingleChar() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        result = testsParser::testLexerAndParser(true, "a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifierSingleChar() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "a"));
        result = testsParser::testLexerAndParser(true, "#a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassSingleChar() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Class, "a"));
        result = testsParser::testLexerAndParser(true, ".a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "b"));
        result = testsParser::testLexerAndParser(true, "a > b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a > #b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::DirectParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "b"));
        result = testsParser::testLexerAndParser(true, "a > .b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "abc"));
        result = testsParser::testLexerAndParser(true, "abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "abc"));
        result = testsParser::testLexerAndParser(true, "#abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::Class, "abc"));
        result = testsParser::testLexerAndParser(true, ".abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingWildcard() {
        style::parser::ParserNode *rootExpected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
            ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList))
            ->addChild(new style::parser::ParserNode(style::parser::Token::StarWildcard));
        result = testsParser::testLexerAndParser(true, "* {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationElementName() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::AnyParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "b"));
        result = testsParser::testLexerAndParser(true, "a b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifier() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::AnyParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a #b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClass() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::AnyParent));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "b"));
        result = testsParser::testLexerAndParser(true, "a .b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a#b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClassStickedToFirstDeclarationPart() {
        style::parser::ParserNode *rootExpected;
        style::parser::ParserNode *expected;
        test::Result result;

        rootExpected = new style::parser::ParserNode(style::parser::Token::NullRoot);
        expected = rootExpected->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsBlock))
                       ->addChild(new style::parser::ParserNode(style::parser::Token::SelectorsList));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::ElementName, "a"));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::SameElement));
        expected->addChild(new style::parser::ParserNode(style::parser::Token::Class, "b"));
        result = testsParser::testLexerAndParser(true, "a.b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    void testsParsingSelectorsBlock(test::Tests *tests) {
        tests->beginTestBlock("Parsing selectors block");
        tests->addTest(testParsingElementNameSingleChar, "Element name single char");
        tests->addTest(testParsingIdentifierSingleChar, "Identifier single char");
        tests->addTest(testParsingClassSingleChar, "Class single char");
        tests->addTest(testParsingElementNameMultipleChars, "Element name multiple chars");
        tests->addTest(testParsingIdentifierMultipleChars, "Identfier multiple chars");
        tests->addTest(testParsingClassMultipleChars, "Class multiple chars");
        tests->addTest(testParsingWildcard, "Wildcard");

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
    }
}
