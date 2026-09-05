#ifndef STYLE_COMPONENT_HPP
#define STYLE_COMPONENT_HPP

#include "../cpp_commons/src/node.hpp"
#include "parser_tokens.hpp"

#include <list>
#include <string>
#include <unordered_map>
#include <utility>

namespace style {

    enum class SelectorType { StarWildcard, ElementName, Class, Modifier, Identifier, Null };

    enum class ValueType { Int, Float, Bool, String, Tuple, Function, Unit, Hex, EnumValue, Null };

    enum class SelectorsRelation { SameElement, DirectParent, AnyParent, Null };

    SelectorType tokenTypeToSelectorType(parser::Token token);
    ValueType tokenTypeToValueType(parser::Token token);
    SelectorsRelation tokenTypeToSelectorsRelation(parser::Token token);

    std::string selectorTypeToString(SelectorType token);
    std::string valueTypeToString(ValueType token);
    std::string selectorsRelationToString(SelectorsRelation token);

    /**
     * Some data types are containing others, for example a tuple contains multiple elements.
     * This class allows representing such elements.
     */
    class StyleValue : public commons::Node<StyleValue> {
        std::string _value;
        ValueType _type;

    public:
        StyleValue(const std::string &value = "", const ValueType type = ValueType::Null) : _value{value}, _type{type} {};
        void value(const std::string &value) { this->_value = value; }
        void type(ValueType type) { this->_type = type; }
        std::string value() const { return _value; }
        ValueType type() const { return _type; }
        StyleValue *copy() const;
        std::string debugValue() const override;
    };

    /**
     *   style value
     *   enabled
     *   specificity
     *   file number
     *   rule number (file dependant)
     */
    struct StyleRule {
        StyleValue *value = nullptr;
        bool enabled;
        int specificity;
        int fileNumber;
        int ruleNumber;

        StyleRule() : value{nullptr}, enabled{false}, specificity{0}, fileNumber{-1}, ruleNumber{-1} {}
        StyleRule(StyleValue *value, bool enabled, int specificity, int fileNumber, int ruleNumber);
        // copy constructor
        StyleRule(const StyleRule &rule);
        // copy assignment operator (same as copy constructor, except it's used when using explicitly the assignment operator ('='))
        StyleRule &operator=(const StyleRule &rule);
        // move constructor
        StyleRule(StyleRule &&rule);
        // move assignment operator (same as move constructor, except it's used when using explicitly the assignment operator ('='))
        StyleRule &operator=(StyleRule &&rule);
        ~StyleRule();
    };

    typedef std::pair<std::string, SelectorType> SelectorData;
    typedef std::list<std::pair<SelectorData, SelectorsRelation>> SelectorDataList;
    typedef std::unordered_map<std::string, StyleRule> RulesMap;
    typedef std::pair<SelectorDataList, RulesMap> StyleDefinition;

} // namespace style

#endif // STYLE_COMPONENT_HPP
