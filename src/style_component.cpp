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

    int StyleValue::nbChilds() const {
        int nb = 0;
        StyleValue *currentChild = this->child;
        while (currentChild != nullptr) {
            nb++;
            currentChild = currentChild->next;
        }
        return nb;
    }

    StyleValue::~StyleValue() {
        delete child;
        delete next;
    }

    StyleValue *StyleValue::copy() const {
        StyleValue *newValue = new StyleValue(value, type);
        if (child != nullptr) newValue->setChild(child->copy());
        if (next != nullptr) newValue->setNext(next->copy());
        return newValue;
    }

    StyleRule::StyleRule(StyleValue *value, bool enabled, int specificity, int fileNumber, int ruleNumber)
        : value{value}, enabled{enabled}, specificity{specificity}, fileNumber{fileNumber}, ruleNumber{ruleNumber} {}

    StyleRule::StyleRule(const StyleRule &rule) {
        if (rule.value != nullptr) value = rule.value->copy();
        enabled = rule.enabled;
        specificity = rule.specificity;
        fileNumber = rule.fileNumber;
        ruleNumber = rule.ruleNumber;
    }

    StyleBlock::StyleBlock(const StyleComponentDataList &componentsList, const StyleValuesMap &styleMap) {
        styleDef = new StyleDefinition(componentsList, styleMap);
    }

    StyleBlock::~StyleBlock() {
        for (std::pair<std::string, StyleRule> rule : styleDef->second) {
            delete rule.second.value;
        }
        delete styleDef;
    }
} // namespace Style
