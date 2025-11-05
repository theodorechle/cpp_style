#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"

namespace style {

    class Node : public commons::Node<Node> {
        Token _type;
        std::string _value;
        void displayTree(std::ostream &flow = std::cout, int level = 0) const;

    public:
        Node(Token type = Token::Empty, std::string value = "", Node *parentNode = nullptr) : _type{type}, _value{value} { parent(parentNode); };
        virtual void value(const std::string &value) { _value = value; };
        virtual const std::string &value() const { return _value; };
        void token(const Token &type) { _type = type; };
        const Token &token() const { return _type; };
        /**
         *Set parent for the node and the nexts nodes
         */
        void setParent(Node *parent);
        Node *getLastChild();
        Node *getSpecificChild(int childNumber);
        /*Set the child pointer*/
        void setChild(Node *childNode);
        void removeFirstChild() { setChild(nullptr); };
        void removeSpecificChild(Node *child);
        void deleteSpecificChild(Node *child);
        Node *appendNext(Node *next);

        /**
            *Replace the data of the node (value, type, childs and nexts)
            *by the ones of the given tree.
            Delete the last childs
        */
        void replaceData(Node *tree);
        void replaceChild(Node *child, Node *newChild);

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