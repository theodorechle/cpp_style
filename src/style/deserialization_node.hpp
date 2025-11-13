#ifndef DESERIALIZATION_NODE_HPP
#define DESERIALIZATION_NODE_HPP

#include <string>

#include "../../cpp_commons/src/node.hpp"
#include "tokens.hpp"

namespace style {

    class DeserializationNode : public commons::Node<DeserializationNode> {
        Token _token;
        std::string _value;
        std::string debugValue() const override;
    public:
        DeserializationNode(Token token = Token::Empty, std::string value = "", DeserializationNode *parentNode = nullptr) : _token{token}, _value{value} { parent(parentNode); };
        void value(const std::string &value) { _value = value; };
        const std::string &value() const { return _value; };
        void token(const Token &token) { _token = token; };
        const Token &token() const { return _token; };
        /**
         *Set parent for the node and the nexts nodes
         */
        void setParent(DeserializationNode *parent);
        DeserializationNode *getLastChild();
        DeserializationNode *getSpecificChild(int childNumber);
        /*Set the child pointer*/
        void setChild(DeserializationNode *childNode);
        void removeFirstChild() { setChild(nullptr); };
        void removeSpecificChild(DeserializationNode *child);
        void deleteSpecificChild(DeserializationNode *child);
        DeserializationNode *appendNext(DeserializationNode *next);

        /**
            *Replace the data of the node (value, token, childs and nexts)
            *by the ones of the given tree.
            Delete the last childs
        */
        void replaceData(DeserializationNode *tree);
        void replaceChild(DeserializationNode *child, DeserializationNode *newChild);

        DeserializationNode *copyNode() const;
        DeserializationNode *copyNodeWithChilds() const;
        DeserializationNode *copyNodeWithChildsAndNexts() const;
        void displayNexts(std::ostream &flow = std::cout) const;

        friend bool operator==(const DeserializationNode &, const DeserializationNode &);
        friend bool operator!=(const DeserializationNode &, const DeserializationNode &);
    };

    /**
     *Return the parent who don't have a parent
     */
    DeserializationNode *root(DeserializationNode *node);

    bool areSameNodes(const DeserializationNode *node1, const DeserializationNode *node2);

    bool isNodeNull(DeserializationNode *node);

} // namespace style

#endif // DESERIALIZATION_NODE_HPP