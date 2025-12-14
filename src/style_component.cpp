#include "style_component.hpp"

namespace style {

    std::string styleComponentTypeToString(StyleComponentType token) {
        switch (token) {
        case StyleComponentType::StarWildcard:
            return "StarWildcard";
        case StyleComponentType::ElementName:
            return "ElementName";
        case StyleComponentType::Class:
            return "Class";
        case StyleComponentType::Modifier:
            return "Modifier";
        case StyleComponentType::Identifier:
            return "Identifier";
        case StyleComponentType::Null:
            return "Null";
        default:
            return "Unknown";
        }
    }

    std::string styleValueTypeToString(StyleValueType token) {
        switch (token) {
        case StyleValueType::Int:
            return "Int";
        case StyleValueType::Float:
            return "Float";
        case StyleValueType::Bool:
            return "Bool";
        case StyleValueType::String:
            return "String";
        case StyleValueType::Tuple:
            return "Tuple";
        case StyleValueType::Function:
            return "Function";
        case StyleValueType::Unit:
            return "Unit";
        case StyleValueType::Hex:
            return "Hex";
        case StyleValueType::EnumValue:
            return "EnumValue";
        case StyleValueType::Null:
            return "Null";
        default:
            return "Unknown";
        }
    }

    std::string styleRelationToString(StyleRelation token) {
        switch (token) {
        case StyleRelation::SameElement:
            return "SameElement";
        case StyleRelation::DirectParent:
            return "DirectParent";
        case StyleRelation::AnyParent:
            return "AnyParent";
        case StyleRelation::Null:
            return "Null";
        default:
            return "Unknown";
        }
    }

    StyleValue *StyleValue::copy() const {
        StyleValue *newValue = new StyleValue(_value, _type);
        if (child() != nullptr) newValue->addChild(child()->copy());
        if (next() != nullptr) newValue->next(next()->copy());
        return newValue;
    }

    std::string StyleValue::debugValue() const { return _value + " (" + styleValueTypeToString(_type) + ")"; }

    StyleRule::StyleRule(StyleValue *value, bool enabled, int specificity, int fileNumber, int ruleNumber)
        : value{value}, enabled{enabled}, specificity{specificity}, fileNumber{fileNumber}, ruleNumber{ruleNumber} {}

    StyleRule::StyleRule(const StyleRule &rule) {
        if (rule.value != nullptr) value = rule.value->copy();
        enabled = rule.enabled;
        specificity = rule.specificity;
        fileNumber = rule.fileNumber;
        ruleNumber = rule.ruleNumber;
    }
} // namespace Style
