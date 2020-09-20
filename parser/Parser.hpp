#ifndef SOVIET_PARSER_HPP
#define SOVIET_PARSER_HPP

#include "../tokenizer/Token.hpp"
#include <vector>
#include "nodes/nodes.hpp"
#include "TokenIterator.hpp"
#include <memory>
#include <stack>
#include <stdexcept>
#include <iostream>

#ifdef DEBUG
#include <exception>
#endif

#include "../util/util.hpp"
#include "ParseError.hpp"

namespace soviet {
    class Parser {
    public:
        std::shared_ptr<Node> parse(TokenIterator&& iterator) {
            this->iterator = std::move(iterator);
            return this->parseExpression();
        }

    private:
        TokenIterator iterator;

        std::shared_ptr<Node> parseExpression() {
            return this->parseComparison();
        }

        std::shared_ptr<Node> parsePrimary() {
            auto& token = iterator.getNextToken();
            switch (token.type) {
                case TokenType::open_bracket:
                    return parseBracketExpression();
                case TokenType::number:
                    return parseNumber(token);
                case TokenType::name:
                    return parseName(token);
                case TokenType::string:
                    return parseString(token);
                default:
                    throw ParseError("unexpected token");
            }
        }

        std::shared_ptr<Node> parseBracketExpression() {
            auto operand = this->parseExpression();

            const auto closeBracket = iterator.getNextToken(); // eat )
            if (closeBracket.type != TokenType::close_bracket)
                throw ParseError("expected \")\"");

            return std::move(operand);
        }

        std::shared_ptr<Node> parseNumber(Token& token) {
            if (!iterator.isEmpty() && !isIn(iterator.peekNextToken().type,
              TokenType::add_op, TokenType::sub_op, TokenType::div_op,
              TokenType::div_op, TokenType::mul_op, TokenType::close_bracket,
              TokenType::double_equals_op, TokenType::comma, TokenType::greater_than))
                throw ParseError("unexpected token");

            return std::make_shared<NumberNode>(
                std::stof(token.value)
            );
        }

        std::shared_ptr<Node> parseIfStatement() {
            auto condition = this->parseExpression();

            const auto thenTok = iterator.getNextToken(); // eat "then"
            if (thenTok.type != TokenType::name || thenTok.value != "then")
                throw ParseError("expected \"then\"");

            auto body = this->parseExpression();

            if (!iterator.isEmpty()) { // "else"
                const auto elseTok = iterator.getNextToken(); // eat "else"
                if (elseTok.type != TokenType::name || elseTok.value != "else")
                    throw ParseError("expected \"else\"");

                auto elseBody = this->parseExpression();
                return std::make_shared<IfNode>(
                    std::move(condition),
                    std::move(body),
                    std::move(elseBody)
                );
            }

            return std::make_shared<IfNode>(
                std::move(condition),
                std::move(body)
            );
        }

        std::shared_ptr<Node> parseString(Token& token) {
            return std::make_shared<StringNode>(
                std::move(token.value)
            );
        }

        std::shared_ptr<Node> parseName(Token& token) {
            if (token.value == "if")
                return parseIfStatement();

            auto node = std::make_shared<NameNode>(
                std::move(token.value)
            );

            if (iterator.isEmpty())
                return node;

            switch (iterator.peekNextToken().type) {
                case TokenType::equals_op:
                    return parseAssignment(std::move(node));
                case TokenType::open_bracket:
                    return parseFunctionCall(std::move(node));
                default:
                    return node;
            }
        }

        std::shared_ptr<Node> parseAssignment(std::shared_ptr<Node>&& name) {
            iterator.getNextToken(); // eat =
            return std::make_shared<EqualsOpNode>(
                std::move(name),
                this->parseExpression()
            );
        }

        std::shared_ptr<Node> parseFunctionCall(std::shared_ptr<Node>&& name) {
            iterator.getNextToken(); // eat (

            std::vector<std::shared_ptr<Node>> args;
            if (iterator.peekNextToken().type != TokenType::close_bracket) {
                while (true) {
                    auto arg = parseExpression();
                    args.push_back(arg);

                    if (iterator.peekNextToken().type == TokenType::close_bracket)
                        break;
                    if (iterator.peekNextToken().type != TokenType::comma)
                        throw ParseError("function arguments must be separated by a comma");

                    iterator.getNextToken(); // eat comma
                }
            }

            const auto closeBracket = iterator.getNextToken(); // eat )
            if (closeBracket.type != TokenType::close_bracket)
                throw ParseError("expected \")\"");

            return std::make_shared<FuncCallNode>(
                std::move(name),
                std::move(args)
            );
        }

        std::shared_ptr<Node> parseComparison() {
            auto operand1 = this->parseAdditive();
            while (iterator.peekNextToken().type == TokenType::double_equals_op) {
                const auto op = iterator.getNextToken();
                auto operand2 = parseAdditive();
                operand1 = std::make_shared<DoubleEqualsOpNode>(
                    std::move(operand1),
                    std::move(operand2)
                );
            }
            return operand1;
        }

        std::shared_ptr<Node> parseAdditive() {
            auto operand1 = this->parseMultiplicative();
            while (isIn(
                iterator.peekNextToken().type,
                TokenType::add_op, TokenType::sub_op
            )) {
                const auto op = iterator.getNextToken();
                auto operand2 = parseMultiplicative();
                if (op.type == TokenType::add_op) {
                    operand1 = std::make_shared<AddOpNode>(
                        std::move(operand1),
                        std::move(operand2)
                    );
                } else {
                    operand1 = std::make_shared<SubOpNode>(
                        std::move(operand1),
                        std::move(operand2)
                    );
                }
            }
            return operand1;
        }

        std::shared_ptr<Node> parseMultiplicative() {
            auto operand1 = this->parsePrimary();

            while (isIn(
                iterator.peekNextToken().type,
                TokenType::mul_op, TokenType::div_op
            )) {
                const auto op = iterator.getNextToken();
                auto operand2 = parsePrimary();
                if (op.type == TokenType::mul_op) {
                    operand1 = std::make_shared<MulOpNode>(
                        std::move(operand1),
                        std::move(operand2)
                    );
                } else {
                    operand1 = std::make_shared<DivOpNode>(
                        std::move(operand1),
                        std::move(operand2)
                    );
                }
            }
            return operand1;
        }
    };
}

#endif //SOVIET_PARSER_HPP
