#include "tests_parser.hpp"
namespace testsParserSelectorsBlock {
    test::Result testParsingElementNameSingleChar() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        result = testsParser::testLexerAndParser(true, "a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }
    test::Result testParsingIdentifierSingleChar() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "a"));
        result = testsParser::testLexerAndParser(true, "#a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassSingleChar() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Class, "a"));
        result = testsParser::testLexerAndParser(true, ".a {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        result = testsParser::testLexerAndParser(true, "a > b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a > #b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingDirectParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::DirectParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        result = testsParser::testLexerAndParser(true, "a > .b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingElementNameMultipleChars() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::ElementName, "abc"));
        result = testsParser::testLexerAndParser(true, "abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingIdentifierMultipleChars() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Identifier, "abc"));
        result = testsParser::testLexerAndParser(true, "#abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingClassMultipleChars() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::Class, "abc"));
        result = testsParser::testLexerAndParser(true, ".abc {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingWildcard() {
        style::DeserializationNode *rootExpected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
            ->addChild(new style::DeserializationNode(style::Token::SelectorsList))
            ->addChild(new style::DeserializationNode(style::Token::StarWildcard));
        result = testsParser::testLexerAndParser(true, "* {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationElementName() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "b"));
        result = testsParser::testLexerAndParser(true, "a b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifier() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a #b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClass() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::AnyParent));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
        result = testsParser::testLexerAndParser(true, "a .b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationIdentifierStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Identifier, "b"));
        result = testsParser::testLexerAndParser(true, "a#b {}", rootExpected, style::parser::ParsingBlock::SELECTORS);
        delete rootExpected;
        return result;
    }

    test::Result testParsingAnyParentRelationClassStickedToFirstDeclarationPart() {
        style::DeserializationNode *rootExpected;
        style::DeserializationNode *expected;
        test::Result result;

        rootExpected = new style::DeserializationNode(style::Token::NullRoot);
        expected = rootExpected->addChild(new style::DeserializationNode(style::Token::SelectorsBlock))
                       ->addChild(new style::DeserializationNode(style::Token::SelectorsList));
        expected->addChild(new style::DeserializationNode(style::Token::ElementName, "a"));
        expected->addChild(new style::DeserializationNode(style::Token::SameElement));
        expected->addChild(new style::DeserializationNode(style::Token::Class, "b"));
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
