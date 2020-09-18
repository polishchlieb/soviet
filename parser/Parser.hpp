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
            return this->parseAdditive();
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
                    throw std::runtime_error(
                        "parsePrimary was called with token of type "
                            + dumpTokenType(token.type)
                            + " which is not defined"
                    );
            }
        }

        std::shared_ptr<Node> parseBracketExpression() {
            auto operand = this->parseExpression();
            this->iterator.getNextToken(); // eat )
            return std::move(operand);
        }

        std::shared_ptr<Node> parseNumber(Token& token) {
            return std::make_shared<NumberNode>(
                std::stof(token.value)
            );
        }

        std::shared_ptr<Node> parseString(Token& token) {
            return std::make_shared<StringNode>(
                std::move(token.value)
            );
        }

        std::shared_ptr<Node> parseName(Token& token) {
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
                        throw std::runtime_error("function arguments must be separated by a comma");

                    iterator.getNextToken(); // eat comma
                }
            }

            iterator.getNextToken(); // eat )

            return std::make_shared<FuncCallNode>(
                std::move(name), std::move(args)
            );
        }

        std::shared_ptr<Node> parseAdditive() {
            auto operand1 = this->parseMultiplicative();
            while (isIn(
                iterator.peekNextToken().type,
                TokenType::add_op, TokenType::sub_op
            )) {
                const auto op = iterator.getNextToken();
                auto operand2 = parseMultiplicative();
                // todo: handle substract operator
                operand1 = std::make_shared<AddOpNode>(
                    std::move(operand1),
                    std::move(operand2)
                );
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
                // todo: handle divide operator
                operand1 = std::make_shared<MulOpNode>(
                    std::move(operand1),
                    std::move(operand2)
                );
            }
            return operand1;
        }
    };
}

#endif //SOVIET_PARSER_HPP
