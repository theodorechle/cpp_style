#include <iostream>

#include "node.hpp"
#include "parser.hpp"

namespace style {

    std::string Node::debugValue() const { return _value + " (" + tokenToString(_token) + ")"; };

    Node *Node::copyNode() const {
        Node *n = new Node;
        n->token(token());
        n->value(value());
        return n;
    }

    /**
     *Copy the node and his childs (not the nexts)
     */
    Node *Node::copyNodeWithChilds() const {
        Node *n = copyNode();
        const Node *childNode = child();
        if (childNode != nullptr) n->setChild(childNode->copyNodeWithChildsAndNexts());
        return n;
    }

    Node *Node::copyNodeWithChildsAndNexts() const {
        Node *n = copyNodeWithChilds();
        const Node *nextNode = next();
        if (nextNode != nullptr) n->next(nextNode->copyNodeWithChildsAndNexts());
        return n;
    }

    void Node::displayNexts(std::ostream &flow) const {
        const Node *next = this;
        while (next != nullptr) {
            flow << next->value() << " (" << next->token() << ")\n";
            next = next->next();
        }
    }

    void Node::setParent(Node *parent) {
        Node *node = this;
        while (node != nullptr) {
            node->parent(parent);
            node = node->next();
        }
    }

    Node *Node::getLastChild() {
        Node *childNode = child();
        Node *nextChild = childNode;
        while (nextChild != nullptr) {
            childNode = nextChild;
            nextChild = nextChild->next();
        }
        return childNode;
    }

    Node *Node::getSpecificChild(int childNumber) {
        Node *childNode = child();
        childNumber--;

        while (childNumber > 0) {
            if (childNode == nullptr) return nullptr;
            childNode = childNode->next();
            childNumber--;
        }
        return childNode;
    }

    void Node::setChild(Node *childNode) {
        removeChilds();
        addChild(childNode);
        if (childNode != nullptr) childNode->setParent(this);
    }

    void Node::removeSpecificChild(Node *childNode) {
        if (childNode == nullptr) return;
        if (childNode == child()) {
            setChild(childNode->next());
            childNode->next(nullptr);
            return;
        }
        Node *nextChild = child();
        while (nextChild != nullptr) {
            if (nextChild->next() == childNode) {
                nextChild->next(childNode->next());
                childNode->next(nullptr);
                return;
            }
            nextChild = nextChild->next();
        }
    }

    void Node::deleteSpecificChild(Node *child) {
        removeSpecificChild(child);
        delete child;
    }

    Node *Node::appendNext(Node *next) {
        if (next == nullptr) return next;
        Node *c = this;
        while (c->next() != nullptr) {
            c = c->next();
        }
        c->next(next);
        next->setParent(parent());
        return next;
    }

    void Node::replaceData(Node *tree) {
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
    void Node::replaceChild(Node *childNode, Node *newChild) {
        if (childNode == nullptr || newChild == nullptr) return;
        Node *c = child();
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

    Node *root(Node *node) {
        if (isNodeNull(node) || isNodeNull(node->parent())) return node;
        return root(node->parent());
    }

    bool areSameNodes(const Node *node1, const Node *node2) {
        if (*node1 != *node2) return false; // works because != is overriden
        if (node1->nbChilds() != node2->nbChilds()) return false;
        const Node *child1 = node1->child();
        const Node *child2 = node2->child();
        while (child1 != nullptr) {
            if (!areSameNodes(child1, child2)) return false;
            child1 = child1->next();
            child2 = child2->next();
        }
        return true;
    }

    bool isNodeNull(Node *node) { return (node == nullptr || node->token() == Token::NullRoot); }

    bool operator==(const Node &n1, const Node &n2) { return (n1.value() == n2.value() && n1.token() == n2.token()); }

    bool operator!=(const Node &n1, const Node &n2) { return (n1.value() != n2.value() || n1.token() != n2.token()); }

} // namespace style
