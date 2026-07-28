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
        : value{value ? value->copy() : nullptr}, enabled{enabled}, specificity{specificity}, fileNumber{fileNumber}, ruleNumber{ruleNumber} {}

    StyleRule::StyleRule(const StyleRule &rule)
        : value{rule.value ? rule.value->copy() : nullptr}, enabled{rule.enabled}, specificity{rule.specificity}, fileNumber{rule.fileNumber}, ruleNumber{rule.ruleNumber} {}

    StyleRule &StyleRule::operator=(const StyleRule &rule) {
        if (this != &rule) {
            if (rule.value) value = rule.value->copy();
            enabled = rule.enabled;
            specificity = rule.specificity;
            fileNumber = rule.fileNumber;
            ruleNumber = rule.ruleNumber;
        }

        return *this;
    }

    StyleRule::StyleRule(StyleRule &&rule)
        : value{rule.value}, enabled{rule.enabled}, specificity{rule.specificity}, fileNumber{rule.fileNumber}, ruleNumber{rule.ruleNumber} {
        rule.value = nullptr;
        rule.fileNumber = -1;
        rule.ruleNumber = -1;
    }

    StyleRule &StyleRule::operator=(StyleRule &&rule) {
        if (this != &rule) {
            value = rule.value;
            enabled = rule.enabled;
            specificity = rule.specificity;
            fileNumber = rule.fileNumber;
            ruleNumber = rule.ruleNumber;
            rule.value = nullptr;
            rule.fileNumber = -1;
            rule.ruleNumber = -1;
        }

        return *this;
    }

    // XXX: By adding this, it crashes
    // The only way I see to get this behavior is if there is some use-after-free
    // Is there another way?
    // Yes, if a pointer to value is first created and then used somewhere else
    StyleRule::~StyleRule() { delete value; }
} // namespace Style
