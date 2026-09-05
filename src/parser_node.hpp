#ifndef PARSER_NODE_HPP
#define PARSER_NODE_HPP

#include <string>

#include "../cpp_commons/src/node.hpp"
#include "parser_tokens.hpp"

namespace style::parser {
    class ParserNode : public commons::Node<ParserNode> {
        Token _token;
        std::string _value;

    protected:
        std::string debugValue() const override;

    public:
        ParserNode(Token token, std::string value = "") : _token{token}, _value{value} {};
        const std::string &value() const { return _value; };
        const Token &token() const { return _token; };

        /*Set the child pointer*/
        void setChild(ParserNode *childNode);

        void removeSpecificChild(ParserNode *child);

        void deleteSpecificChild(ParserNode *child);

        void replaceChild(ParserNode *child, ParserNode *newChild);

        ParserNode *copyNode() const;
        ParserNode *copyNodeWithChildren() const;
        ParserNode *copyNodeWithChildrenAndNexts() const;
    };

    bool isNodeNull(ParserNode *node);

    bool areSameNodes(const ParserNode *node1, const ParserNode *node2);
} // style::parser

#endif // PARSER_NODE_HPP
