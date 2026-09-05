#include "lexer_node.hpp"

namespace style::lexer {
    std::string LexerNode::debugValue() const { return _value + " (" + tokenToString(_token) + ")"; };

    void LexerNode::setTextPosition(size_t line, size_t column) {
        _line = line;
        _column = column;
    }

    void LexerNode::displayNexts(std::ostream &flow) const {
        const LexerNode *next = this;
        while (next != nullptr) {
            flow << next->value() << " (" << tokenToString(next->token()) << ")\n";
            next = next->next();
        }
    }

    bool areSameNodes(const LexerNode *node1, const LexerNode *node2) {
        if (!node1 && !node2) return true;
        if (!node1 || !node2) return false;
        if (node1->token() != node2->token() || node1->value() != node2->value()) return false;
        return areSameNodes(node1->child(), node2->child()) && areSameNodes(node1->next(), node2->next());
    }
} // style::lexer
