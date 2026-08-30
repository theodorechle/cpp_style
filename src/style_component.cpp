#include "style_component.hpp"

namespace style {

    std::string selectorTypeToString(SelectorType token) {
        switch (token) {
        case SelectorType::StarWildcard:
            return "StarWildcard";
        case SelectorType::ElementName:
            return "ElementName";
        case SelectorType::Class:
            return "Class";
        case SelectorType::Modifier:
            return "Modifier";
        case SelectorType::Identifier:
            return "Identifier";
        case SelectorType::Null:
            return "Null";
        default:
            return "Unknown";
        }
    }

    std::string valueTypeToString(ValueType token) {
        switch (token) {
        case ValueType::Int:
            return "Int";
        case ValueType::Float:
            return "Float";
        case ValueType::Bool:
            return "Bool";
        case ValueType::String:
            return "String";
        case ValueType::Tuple:
            return "Tuple";
        case ValueType::Function:
            return "Function";
        case ValueType::Unit:
            return "Unit";
        case ValueType::Hex:
            return "Hex";
        case ValueType::EnumValue:
            return "EnumValue";
        case ValueType::Null:
            return "Null";
        default:
            return "Unknown";
        }
    }

    std::string selectorsRelationToString(SelectorsRelation token) {
        switch (token) {
        case SelectorsRelation::SameElement:
            return "SameElement";
        case SelectorsRelation::DirectParent:
            return "DirectParent";
        case SelectorsRelation::AnyParent:
            return "AnyParent";
        case SelectorsRelation::Null:
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

    std::string StyleValue::debugValue() const { return _value + " (" + valueTypeToString(_type) + ")"; }

    StyleRule::StyleRule(StyleValue *value, bool enabled, int specificity, int fileNumber, int ruleNumber)
        : value{value ? value->copy() : nullptr}, enabled{enabled}, specificity{specificity}, fileNumber{fileNumber}, ruleNumber{ruleNumber} {}

    StyleRule::StyleRule(const StyleRule &rule)
        : value{rule.value ? rule.value->copy() : nullptr}, enabled{rule.enabled}, specificity{rule.specificity}, fileNumber{rule.fileNumber},
          ruleNumber{rule.ruleNumber} {}

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

    StyleRule::~StyleRule() { delete value; }
} // namespace Style
