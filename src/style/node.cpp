#include <iostream>

#include "node.hpp"
#include "parser.hpp"

namespace style {

    Node *Node::copyNode() const {
        Node *n = createNewNode();
        n->setToken(getToken());
        n->setValue(getValue());
        return n;
    }

    /**
     *Copy the node and his childs (not the nexts)
     */
    Node *Node::copyNodeWithChilds() const {
        Node *n = copyNode();
        Node *child = getChild();
        if (child != nullptr) n->setChild(child->copyNodeWithChildsAndNexts());
        return n;
    }

    Node *Node::copyNodeWithChildsAndNexts() const {
        Node *n = copyNodeWithChilds();
        Node *next = getNext();
        if (next != nullptr) n->setNext(next->copyNodeWithChildsAndNexts());
        return n;
    }

    void Node::displayTree(std::ostream &flow, int level) const {
        for (int i = 0; i < level; i++)
            flow << "\t";
        flow << getValue() << " (" << getToken() << ")\n";
        const Node *child = getChild();
        while (child != nullptr) {
            child->displayTree(flow, level + 1);
            child = child->next;
        }
    }

    void Node::display(std::ostream &flow) const { displayTree(flow, 0); }

    void Node::displayNexts(std::ostream &flow) const {
        const Node *next = this;
        while (next != nullptr) {
            flow << next->getValue() << " (" << next->getToken() << ")\n";
            next = next->getNext();
        }
    }

    void Node::setParent(Node *parent) {
        Node *node = this;
        while (node != nullptr) {
            node->parent = parent;
            node = node->getNext();
        }
    }

    Node *Node::getLastChild() const {
        Node *child = getChild();
        Node *nextChild = child;
        while (nextChild != nullptr) {
            child = nextChild;
            nextChild = nextChild->getNext();
        }
        return child;
    }

    Node *Node::getSpecificChild(int childNumber) const {
        Node *child = getChild();
        childNumber--;

        while (childNumber > 0) {
            if (child == nullptr) return nullptr;
            child = child->getNext();
            childNumber--;
        }
        return child;
    }

    int Node::getNbChilds() const {
        int nbChilds = 0;
        Node *child = getChild();
        while (child != nullptr) {
            nbChilds++;
            child = child->getNext();
        }
        return nbChilds;
    }

    void Node::setChild(Node *child) {
        this->child = child;
        if (child != nullptr) child->setParent(this);
    }

    void Node::removeSpecificChild(Node *child) {
        if (child == nullptr) return;
        if (child == getChild()) {
            setChild(child->getNext());
            child->setNext(nullptr);
            return;
        }
        Node *nextChild = getChild();
        while (nextChild != nullptr) {
            if (nextChild->getNext() == child) {
                nextChild->setNext(child->getNext());
                child->setNext(nullptr);
                return;
            }
            nextChild = nextChild->getNext();
        }
    }

    void Node::deleteSpecificChild(Node *child) {
        removeSpecificChild(child);
        delete child;
    }

    Node *Node::appendNext(Node *next) {
        if (next == nullptr) return next;
        Node *c = this;
        while (c->getNext() != nullptr) {
            c = c->getNext();
        }
        c->setNext(next);
        next->setParent(getParent());
        return next;
    }

    Node *Node::appendChild(Node *child) {
        Node *c = getChild();
        if (c == nullptr) setChild(child);
        else c->appendNext(child);
        return child;
    }

    Node *Node::addEmptyChild() {
        Node *child = new Node();
        return appendChild(child);
    }

    void Node::replaceData(Node *tree) {
        if (tree == nullptr) return;
        tree = tree->copyNodeWithChildsAndNexts();
        // copy node
        setValue(tree->getValue());
        setToken(tree->getToken());
        // replace child
        delete getChild();
        setChild(tree->getChild());
        if (getChild() != nullptr) {
            getChild()->setParent(this);
            tree->setChild(nullptr);
        }
        // replace next
        delete getNext();
        setNext(tree->getNext());
        tree->setNext(nullptr);

        delete tree;
    }

    /**
     *Find the child in the childs of the node and replace it with the new one
     */
    void Node::replaceChild(Node *child, Node *newChild) {
        if (child == nullptr || newChild == nullptr) return;
        Node *c = getChild();
        delete newChild->getNext();
        if (c == child) {
            newChild->setNext(c->getNext());
            setChild(newChild);
            c->setNext(nullptr);
            delete c;
            return;
        }
        while (c != nullptr) {
            if (c->getNext() == child) {
                newChild->setNext(c->getNext()->getNext());
                c->setNext(newChild);
                newChild->setParent(this);
                child->setNext(nullptr);
                delete child;
                return;
            }
            c = c->getNext();
        }
    }

    Node::~Node() {
        delete next;
        delete child;
    }

    Node *root(Node *node) {
        if (isNodeNull(node) || isNodeNull(node->getParent())) return node;
        return root(node->getParent());
    }

    bool areSameNodes(const Node *node1, const Node *node2) {
        if (*node1 != *node2) return false; // works because != is overrided
        if (node1->getNbChilds() != node2->getNbChilds()) return false;
        Node *child1 = node1->getChild();
        Node *child2 = node2->getChild();
        while (child1 != nullptr) {
            if (!areSameNodes(child1, child2)) return false;
            child1 = child1->getNext();
            child2 = child2->getNext();
        }
        return true;
    }

    bool areSameNodes(const Node &node1, const Node &node2) { return areSameNodes(&node1, &node2); }

    void deleteNullRoot(Node *node) {
        node = root(node);
        if (node->getParent() == nullptr) return;
        node->getParent()->setChild(nullptr);
        delete node->getParent();
        node->setParent(nullptr);
    }

    bool isNodeNull(Node *node) { return (node == nullptr || node->getToken() == Token::NullRoot); }

    bool operator==(const Node &n1, const Node &n2) { return (n1.getValue() == n2.getValue() && n1.getToken() == n2.getToken()); }

    bool operator!=(const Node &n1, const Node &n2) { return (n1.getValue() != n2.getValue() || n1.getToken() != n2.getToken()); }

} // namespace style
