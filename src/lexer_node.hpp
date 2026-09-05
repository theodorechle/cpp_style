#ifndef LEXER_NODE_HPP
#define LEXER_NODE_HPP
#include <string>

#include "../cpp_commons/src/node.hpp"
#include "lexer_tokens.hpp"

namespace style::lexer {
    class LexerNode : public commons::Node<LexerNode> {
        Token _token;
        std::string _value;
        size_t _line = 0;
        size_t _column = 0;

    protected:
        std::string debugValue() const override;

    public:
        LexerNode(Token token, std::string value = "") : _token{token}, _value{value} {};

        const std::string &value() const { return _value; };
        const Token &token() const { return _token; };
        size_t line() const { return _line; };
        size_t column() const { return _column; };

        void setTextPosition(size_t line, size_t column);

        void displayNexts(std::ostream &flow = std::cout) const;
    };

    bool areSameNodes(const LexerNode *node1, const LexerNode *node2);
} // style::lexer

#endif // LEXER_NODE_HPP
