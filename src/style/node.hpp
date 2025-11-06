#ifndef NODE_HPP
#define NODE_HPP

#include <string>

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"

namespace style {

    class Node : public commons::Node<Node> {
        Token _token;
        std::string _value;
        std::string debugValue() const override;
    public:
        Node(Token token = Token::Empty, std::string value = "", Node *parentNode = nullptr) : _token{token}, _value{value} { parent(parentNode); };
        void value(const std::string &value) { _value = value; };
        const std::string &value() const { return _value; };
        void token(const Token &token) { _token = token; };
        const Token &token() const { return _token; };
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
            *Replace the data of the node (value, token, childs and nexts)
            *by the ones of the given tree.
            Delete the last childs
        */
        void replaceData(Node *tree);
        void replaceChild(Node *child, Node *newChild);

        Node *copyNode() const;
        Node *copyNodeWithChilds() const;
        Node *copyNodeWithChildsAndNexts() const;
        void displayNexts(std::ostream &flow = std::cout) const;

        friend bool operator==(const Node &, const Node &);
        friend bool operator!=(const Node &, const Node &);
    };

    /**
     *Return the parent who don't have a parent
     */
    Node *root(Node *node);

    bool areSameNodes(const Node *node1, const Node *node2);

    bool isNodeNull(Node *node);

} // namespace style

#endif // NODE_HPP