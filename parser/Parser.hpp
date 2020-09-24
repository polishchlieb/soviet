#ifndef SOVIET_PARSER_HPP
#define SOVIET_PARSER_HPP

#include "../tokenizer/Token.hpp"
#include <vector>
#include "nodes/nodes.hpp"
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
        std::shared_ptr<Node> parse() {
            return this->parseExpression();
        }

        Tokenizer tokenizer;
    private:
        std::shared_ptr<Node> parseExpression() {
            return this->parseComparison();
        }

        std::shared_ptr<Node> parsePrimary() {
            auto token = tokenizer.getNextToken();
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
            if (tokenizer.peekNextToken().type == TokenType::close_bracket) {
                tokenizer.getNextToken(); // eat close bracket

                const auto arrowTok = tokenizer.getNextToken(); // eat ->
                if (arrowTok.type != TokenType::arrow)
                    throw ParseError("expected \"->\"");

                auto returnValue = this->parseExpression();

                return std::make_shared<PrototypeNode>(
                    std::vector<std::shared_ptr<Node>>{},
                    std::move(returnValue)
                );
            }

            auto operand = this->parseExpression();
            if (tokenizer.peekNextToken().type == TokenType::comma) {
                std::vector<std::shared_ptr<Node>> args{operand};
                while (tokenizer.peekNextToken().type == TokenType::comma) {
                    tokenizer.getNextToken(); // eat comma
                    args.push_back(this->parseExpression());
                }

                const auto closeBracket = tokenizer.getNextToken(); // eat )
                if (closeBracket.type != TokenType::close_bracket)
                    throw ParseError("expected \")\"");

                const auto arrow = tokenizer.getNextToken(); // eat ->
                if (arrow.type != TokenType::arrow)
                    throw ParseError("expected \"->\"");

                return std::make_shared<PrototypeNode>(
                    std::move(args),
                    this->parseExpression()
                );
            }

            const auto closeBracket = tokenizer.getNextToken(); // eat )
            if (closeBracket.type != TokenType::close_bracket)
                throw ParseError("expected \")\"");

            if (!tokenizer.isEmpty()) {
                switch (tokenizer.peekNextToken().type) {
                    case TokenType::arrow:
                        tokenizer.getNextToken(); // eat ->

                        return std::make_shared<PrototypeNode>(
                            std::vector{operand},
                            this->parseExpression()
                        );
                    case TokenType::open_bracket:
                        return parseFunctionCall(std::move(operand));
                    default:
                        break;
                }
            }

            return std::move(operand);
        }

        std::shared_ptr<Node> parseNumber(Token& token) {
            return std::make_shared<NumberNode>(
                std::stof(token.value)
            );
        }

        std::shared_ptr<Node> parseIfStatement() {
            auto condition = this->parseExpression();

            const auto thenTok = tokenizer.getNextToken(); // eat "then"
            if (thenTok.type != TokenType::name || thenTok.value != "then")
                throw ParseError("expected \"then\"");

            auto body = this->parseExpression();

            if (!tokenizer.isEmpty()) { // "else"
                const auto elseTok = tokenizer.getNextToken(); // eat "else"
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

            if (tokenizer.isEmpty())
                return node;

            switch (tokenizer.peekNextToken().type) {
                case TokenType::equals_op:
                    return parseAssignment(std::move(node));
                case TokenType::open_bracket:
                    return parseFunctionCall(std::move(node));
                case TokenType::arrow: {
                    tokenizer.getNextToken(); // eat ->

                    auto returnValue = this->parseExpression();

                    return std::make_shared<PrototypeNode>(
                        std::vector<std::shared_ptr<Node>>{
                            std::move(node)
                        },
                        std::move(returnValue)
                    );
                }
                default:
                    return node;
            }
        }

        std::shared_ptr<Node> parseAssignment(std::shared_ptr<Node>&& name) {
            tokenizer.getNextToken(); // eat =
            return std::make_shared<EqualsOpNode>(
                std::move(name),
                this->parseExpression()
            );
        }

        std::shared_ptr<Node> parseFunctionCall(std::shared_ptr<Node>&& name) {
            tokenizer.getNextToken(); // eat (

            std::vector<std::shared_ptr<Node>> args;
            if (tokenizer.peekNextToken().type != TokenType::close_bracket) {
                while (true) {
                    auto arg = parseExpression();
                    args.push_back(arg);

                    if (tokenizer.peekNextToken().type == TokenType::close_bracket)
                        break;
                    if (tokenizer.peekNextToken().type != TokenType::comma)
                        throw ParseError("function arguments must be separated by a comma");

                    tokenizer.getNextToken(); // eat comma
                }
            }

            const auto closeBracket = tokenizer.getNextToken(); // eat )
            if (closeBracket.type != TokenType::close_bracket)
                throw ParseError("expected \")\"");

            return std::make_shared<FuncCallNode>(
                std::move(name),
                std::move(args)
            );
        }

        std::shared_ptr<Node> parseComparison() {
            auto operand1 = this->parseAdditive();
            while (!tokenizer.isEmpty()
              && tokenizer.peekNextToken().type == TokenType::double_equals_op) {
                const auto op = tokenizer.getNextToken();
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
            while (!tokenizer.isEmpty() && isIn(
                tokenizer.peekNextToken().type,
                TokenType::add_op, TokenType::sub_op
            )) {
                const auto op = tokenizer.getNextToken();
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

            while (!tokenizer.isEmpty() && isIn(
                tokenizer.peekNextToken().type,
                TokenType::mul_op, TokenType::div_op
            )) {
                const auto op = tokenizer.getNextToken();
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
