#include <iostream>

#include "deserialization_node.hpp"
#include "parser.hpp"

namespace style {

    std::string DeserializationNode::debugValue() const { return _value + " (" + tokenToString(_token) + ")"; };

    DeserializationNode *DeserializationNode::copyNode() const {
        DeserializationNode *n = new DeserializationNode;
        n->token(token());
        n->value(value());
        return n;
    }

    /**
     *Copy the node and his childs (not the nexts)
     */
    DeserializationNode *DeserializationNode::copyNodeWithChilds() const {
        DeserializationNode *n = copyNode();
        const DeserializationNode *childNode = child();
        if (childNode != nullptr) n->setChild(childNode->copyNodeWithChildsAndNexts());
        return n;
    }

    DeserializationNode *DeserializationNode::copyNodeWithChildsAndNexts() const {
        DeserializationNode *n = copyNodeWithChilds();
        const DeserializationNode *nextNode = next();
        if (nextNode != nullptr) n->next(nextNode->copyNodeWithChildsAndNexts());
        return n;
    }

    void DeserializationNode::displayNexts(std::ostream &flow) const {
        const DeserializationNode *next = this;
        while (next != nullptr) {
            flow << next->value() << " (" << next->token() << ")\n";
            next = next->next();
        }
    }

    void DeserializationNode::setParent(DeserializationNode *parent) {
        DeserializationNode *node = this;
        while (node != nullptr) {
            node->parent(parent);
            node = node->next();
        }
    }

    DeserializationNode *DeserializationNode::getLastChild() {
        DeserializationNode *childNode = child();
        DeserializationNode *nextChild = childNode;
        while (nextChild != nullptr) {
            childNode = nextChild;
            nextChild = nextChild->next();
        }
        return childNode;
    }

    DeserializationNode *DeserializationNode::getSpecificChild(int childNumber) {
        DeserializationNode *childNode = child();
        childNumber--;

        while (childNumber > 0) {
            if (childNode == nullptr) return nullptr;
            childNode = childNode->next();
            childNumber--;
        }
        return childNode;
    }

    void DeserializationNode::setChild(DeserializationNode *childNode) {
        removeChilds();
        addChild(childNode);
        if (childNode != nullptr) childNode->setParent(this);
    }

    void DeserializationNode::removeSpecificChild(DeserializationNode *childNode) {
        if (childNode == nullptr) return;
        if (childNode == child()) {
            setChild(childNode->next());
            childNode->next(nullptr);
            return;
        }
        DeserializationNode *nextChild = child();
        while (nextChild != nullptr) {
            if (nextChild->next() == childNode) {
                nextChild->next(childNode->next());
                childNode->next(nullptr);
                return;
            }
            nextChild = nextChild->next();
        }
    }

    void DeserializationNode::deleteSpecificChild(DeserializationNode *child) {
        removeSpecificChild(child);
        delete child;
    }

    DeserializationNode *DeserializationNode::appendNext(DeserializationNode *next) {
        if (next == nullptr) return next;
        DeserializationNode *c = this;
        while (c->next() != nullptr) {
            c = c->next();
        }
        c->next(next);
        next->setParent(parent());
        return next;
    }

    void DeserializationNode::replaceData(DeserializationNode *tree) {
        if (tree == nullptr) return;
        tree = tree->copyNodeWithChildsAndNexts();
        // copy node
        value(tree->value());
        token(tree->token());
        // replace child
        delete child();
        setChild(tree->child());
        if (child() != nullptr) {
            child()->setParent(this);
            tree->setChild(nullptr);
        }
        // replace next
        delete next();
        next(tree->next());
        tree->next(nullptr);

        delete tree;
    }

    /**
     *Find the child in the childs of the node and replace it with the new one
     */
    void DeserializationNode::replaceChild(DeserializationNode *childNode, DeserializationNode *newChild) {
        if (childNode == nullptr || newChild == nullptr) return;
        DeserializationNode *c = child();
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
                newChild->setParent(this);
                childNode->next(nullptr);
                delete childNode;
                return;
            }
            c = c->next();
        }
    }

    DeserializationNode *root(DeserializationNode *node) {
        if (isNodeNull(node) || isNodeNull(node->parent())) return node;
        return root(node->parent());
    }

    bool areSameNodes(const DeserializationNode *node1, const DeserializationNode *node2) {
        if (*node1 != *node2) return false; // works because != is overriden
        if (node1->nbChilds() != node2->nbChilds()) return false;
        const DeserializationNode *child1 = node1->child();
        const DeserializationNode *child2 = node2->child();
        while (child1 != nullptr) {
            if (!areSameNodes(child1, child2)) return false;
            child1 = child1->next();
            child2 = child2->next();
        }
        return true;
    }

    bool isNodeNull(DeserializationNode *node) { return (node == nullptr || node->token() == Token::NullRoot); }

    bool operator==(const DeserializationNode &n1, const DeserializationNode &n2) { return (n1.value() == n2.value() && n1.token() == n2.token()); }

    bool operator!=(const DeserializationNode &n1, const DeserializationNode &n2) { return (n1.value() != n2.value() || n1.token() != n2.token()); }

} // namespace style
