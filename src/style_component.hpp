#ifndef STYLE_COMPONENT_HPP
#define STYLE_COMPONENT_HPP

#include "../cpp_commons/src/node.hpp"
#include "tokens.hpp"

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace style {

    enum class StyleComponentType { StarWildcard, ElementName, Class, Modifier, Identifier, Null };

    enum class StyleValueType { Int, Float, Bool, String, Tuple, Function, Unit, Hex, EnumValue, Null };

    enum class StyleRelation { SameElement, DirectParent, AnyParent, Null };

    StyleComponentType tokenTypeToStyleComponentType(Token token);
    StyleValueType tokenTypeToStyleValueType(Token token);
    StyleRelation tokenTypeToStyleRelation(Token token);

    std::string styleComponentTypeToString(StyleComponentType token);
    std::string styleValueTypeToString(StyleValueType token);
    std::string styleRelationToString(StyleRelation token);

    /**
     * Some data types are containing others, for example a tuple contains multiple elements.
     * This class allows representing such elements.
     */
    class StyleValue : public commons::Node<StyleValue> {
        std::string _value;
        StyleValueType _type;

    public:
        StyleValue(const std::string &value = "", const StyleValueType type = StyleValueType::Null) : _value{value}, _type{type} {};
        void value(const std::string &value) { this->_value = value; }
        void type(StyleValueType type) { this->_type = type; }
        std::string value() const { return _value; }
        StyleValueType type() const { return _type; }
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
        StyleRule(const StyleRule &rule);
    };

    typedef std::pair<std::string, StyleComponentType> StyleComponentData;
    typedef std::list<std::pair<StyleComponentData, StyleRelation>> StyleComponentDataList;
    typedef std::unordered_map<std::string, StyleRule> StyleValuesMap;
    typedef std::pair<StyleComponentDataList, StyleValuesMap> StyleDefinition;

    typedef std::unordered_map<std::string, StyleRule> RulesMap;

} // namespace style

#endif // STYLE_COMPONENT_HPP
