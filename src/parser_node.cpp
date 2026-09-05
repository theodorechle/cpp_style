#include "parser_node.hpp"
#include "parser_tokens.hpp"

namespace style::parser {
    std::string ParserNode::debugValue() const { return _value + " (" + tokenToString(_token) + ")"; };

    void ParserNode::setChild(ParserNode *childNode) {
        removeChildren();
        addChild(childNode);
        if (childNode != nullptr) childNode->setParentForCurrentAndNexts(this);
    }

    void ParserNode::removeSpecificChild(ParserNode *childNode) {
        if (childNode == nullptr) return;
        if (childNode == child()) {
            setChild(childNode->next());
            childNode->next(nullptr);
            return;
        }
        ParserNode *nextChild = child();
        while (nextChild != nullptr) {
            if (nextChild->next() == childNode) {
                nextChild->next(childNode->next());
                childNode->next(nullptr);
                return;
            }
            nextChild = nextChild->next();
        }
    }

    void ParserNode::deleteSpecificChild(ParserNode *child) {
        removeSpecificChild(child);
        delete child;
    }

    /**
     *Find the child in the children of the node and replace it with the new one
     */
    void ParserNode::replaceChild(ParserNode *childNode, ParserNode *newChild) {
        if (childNode == nullptr || newChild == nullptr) return;
        ParserNode *c = child();
        delete newChild->next();
        if (c == childNode) {
            newChild->next(c->next());
            setChild(newChild);
            c->next(nullptr);
            delete c;
            return;
        }
        while (c != nullptr) {
            if (c->next() == childNode) {
                newChild->next(c->next()->next());
                c->next(newChild);
                newChild->setParentForCurrentAndNexts(this);
                childNode->next(nullptr);
                delete childNode;
                return;
            }
            c = c->next();
        }
    }

    ParserNode *ParserNode::copyNode() const { return new ParserNode(token(), value()); }

    /**
     *Copy the node and his children (not the nexts)
     */
    ParserNode *ParserNode::copyNodeWithChildren() const {
        ParserNode *n = copyNode();
        const ParserNode *childNode = child();
        if (childNode != nullptr) n->setChild(childNode->copyNodeWithChildrenAndNexts());
        return n;
    }

    ParserNode *ParserNode::copyNodeWithChildrenAndNexts() const {
        ParserNode *n = copyNodeWithChildren();
        const ParserNode *nextNode = next();
        if (nextNode != nullptr) n->next(nextNode->copyNodeWithChildrenAndNexts());
        return n;
    }

    bool isNodeNull(ParserNode *node) { return (node == nullptr || node->token() == Token::NullRoot); }

    bool areSameNodes(const ParserNode *node1, const ParserNode *node2) {
        if (!node1 && !node2) return true;
        if (!node1 || !node2) return false;
        if (node1->token() != node2->token() || node1->value() != node2->value()) return false;
        return areSameNodes(node1->child(), node2->child()) && areSameNodes(node1->next(), node2->next());
    }
} // style::parser
