#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>

#include "tokens.hpp"

namespace style {

    class Node {
        Token type;
        std::string value;
        Node *next = nullptr;
        Node *child = nullptr;
        Node *parent;
        void displayTree(std::ostream &flow = std::cout, int level = 0) const;

    public:
        Node(Token type = Token::Empty, std::string value = "", Node *parent = nullptr) : type{type}, value{value}, parent{parent} {};
        virtual ~Node();
        virtual void setValue(const std::string &s) { value = s; };
        virtual const std::string &getValue() const { return value; };
        void setToken(const Token &t) { type = t; };
        const Token &getToken() const { return type; };
        Node *getParent() const { return parent; };
        /**
         *Set parent for the node and the nexts nodes
         */
        void setParent(Node *parent);
        Node *getNext() const { return next; };
        void setNext(Node *n) { next = n; };
        void removeNext() { setNext(nullptr); };
        Node *getChild() const { return child; };
        Node *getLastChild() const;
        Node *getSpecificChild(int childNumber) const;
        int getNbChilds() const;
        /*Set the child pointer*/
        void setChild(Node *child);
        void removeFirstChild() { setChild(nullptr); };
        void removeSpecificChild(Node *child);
        void deleteSpecificChild(Node *child);
        Node *appendNext(Node *next);
        /**add a child at the end of them
         *Return the child
         */
        Node *appendChild(Node *child);
        Node *addEmptyChild();

        /**
            *Replace the data of the node (value, type, childs and nexts)
            *by the ones of the given tree.
            Delete the last childs
        */
        void replaceData(Node *tree);
        void replaceChild(Node *child, Node *newChild);

        virtual Node *createNewNode() const { return new Node; }
        Node *copyNode() const;
        Node *copyNodeWithChilds() const;
        Node *copyNodeWithChildsAndNexts() const;
        void display(std::ostream &flow = std::cout) const;
        void displayNexts(std::ostream &flow = std::cout) const;

        friend bool operator==(const Node &, const Node &);
        friend bool operator!=(const Node &, const Node &);
    };

    /**
     *Return the parent who don't have a parent
     */
    Node *root(Node *node);

    bool areSameNodes(const Node *node1, const Node *node2);
    bool areSameNodes(const Node &node1, const Node &node2);

    void deleteNullRoot(Node *node);

    bool isNodeNull(Node *node);

} // namespace style

#endif // NODE_HPP