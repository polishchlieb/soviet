#ifndef SOVIET_PARSER_HPP
#define SOVIET_PARSER_HPP

#include "../tokenizer/Token.hpp"
#include <utility>
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
#include "../tokenizer/Tokenizer.hpp"

namespace soviet {
    template<typename T, typename std::enable_if<std::is_base_of<Tokenizer, T>::value>::type* = nullptr>
    class Parser {
    public:
        template<typename ...Args>
        explicit Parser(Args... args) : tokenizer{args...}
        {}

        std::shared_ptr<Node> parse() {
            return this->parseExpression();
        }

        T tokenizer;
    private:
        std::shared_ptr<Node> parseExpression() {
            return this->parseAssignment();
        }

        std::shared_ptr<Node> parsePrimary() {
            std::shared_ptr<Node> result;
            switch (tokenizer.peekNextToken().type) {
                case TokenType::open_bracket:
                    return parseBracketExpression();
                case TokenType::open_curly_bracket:
                    return parseCurlyBracketExpression();
                case TokenType::number:
                    return parseNumber();
                case TokenType::name:
                    return parseName();
                case TokenType::string:
                    return parseString();
                default: {
                    const auto& token = tokenizer.peekNextToken();
                    throw ParseError("unexpected token " + token.value + " on line " + std::to_string(token.line));
                }
            }
        }

        std::shared_ptr<Node> parseBracketExpression() {
            tokenizer.getNextToken(); // eat open bracket

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

            if (!tokenizer.isEmpty()
                && tokenizer.peekNextToken().type == TokenType::arrow) {
                tokenizer.getNextToken(); // eat ->

                return std::make_shared<PrototypeNode>(
                    std::vector{operand},
                    this->parseExpression()
                );
            }

            return operand;
        }

        std::shared_ptr<Node> parseCurlyBracketExpression() {
            tokenizer.getNextToken(); // eat curly bracket

            std::vector<std::shared_ptr<Node>> nodes;
            while (tokenizer.peekNextToken().type != TokenType::close_curly_bracket) {
                nodes.push_back(this->parseExpression());
            }
            tokenizer.getNextToken(); // eat close curly bracket
            return std::make_shared<BlockNode>(std::move(nodes));
        }

        std::shared_ptr<Node> parseNumber() {
            auto result = std::make_shared<NumberNode>(
                std::stof(tokenizer.getNextToken().value)
            );

            return result;
        }

        std::shared_ptr<Node> parseIfStatement() {
            auto condition = this->parseExpression();

            auto body = this->parseExpression();

            if (!tokenizer.isEmpty()
                && tokenizer.peekNextToken().type == TokenType::name
                && tokenizer.peekNextToken().value == "else") { // "else"
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

        std::shared_ptr<Node> parseString() {
            return std::make_shared<StringNode>(
                std::move(tokenizer.getNextToken().value)
            );
        }

        std::shared_ptr<Node> parseReturn() {
            return std::make_shared<ReturnNode>(
                this->parseExpression()
            );
        }

        std::shared_ptr<Node> parseImport() {
            auto moduleName = tokenizer.getNextToken();
            if (!isIn(moduleName.type, TokenType::name, TokenType::string))
                throw ParseError("expected a name or a string");

            std::shared_ptr<Node> module;
            switch (moduleName.type) {
                case TokenType::name:
                    module = std::make_shared<NameNode>(std::move(moduleName.value));
                    break;
                case TokenType::string:
                    module = std::make_shared<StringNode>(std::move(moduleName.value));
                    break;
            }

            return std::make_shared<ImportNode>(std::move(module));
        }

        std::shared_ptr<Node> parseName() {
            auto token = tokenizer.getNextToken();

            if (token.value == "if")
                return parseIfStatement();
            if (token.value == "return")
                return parseReturn();
            if (token.value == "import")
                return parseImport();

            auto node = std::make_shared<NameNode>(
                std::move(token.value)
            );

            if (tokenizer.isEmpty())
                return node;

            switch (tokenizer.peekNextToken().type) {
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

        std::shared_ptr<Node> parseFunctionCallOrDotOperator() {
            auto result = parsePrimary();

            while (!tokenizer.isEmpty() && isIn(
                tokenizer.peekNextToken().type,
                TokenType::open_bracket, TokenType::dot
            )) {
                switch (tokenizer.getNextToken().type) {
                    case TokenType::open_bracket: {
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

                        result = std::make_shared<FuncCallNode>(
                                std::move(result),
                                std::move(args)
                        );
                        break;
                    }
                    case TokenType::dot: {
                        auto operand2 = parseName();

                        result = std::make_shared<DotOpNode>(
                                std::move(result),
                                std::move(operand2)
                        );
                        break;
                    }
                    default:
                        break;
                }
            }

            return result;
        }

        std::shared_ptr<Node> parseAssignment() {
            auto operand1 = parseComparison();
            while (!tokenizer.isEmpty() && tokenizer.peekNextToken().type == TokenType::equals_op) {
                const auto op = tokenizer.getNextToken();
                auto operand2 = parseComparison();
                operand1 = std::make_shared<EqualsOpNode>(
                    std::move(operand1), std::move(operand2)
                );
            }
            return operand1;
        }

        std::shared_ptr<Node> parseComparison() {
            auto operand1 = this->parseAdditive();
            while (!tokenizer.isEmpty() && isIn(
                tokenizer.peekNextToken().type,
                TokenType::double_equals_op, TokenType::greater_than,
                TokenType::less_than
            )) {
                const auto op = tokenizer.getNextToken();
                auto operand2 = parseAdditive();
                switch (op.type) {
                    case TokenType::double_equals_op: {
                        operand1 = std::make_shared<DoubleEqualsOpNode>(
                            std::move(operand1),
                            std::move(operand2)
                        );
                        break;
                    }
                    case TokenType::greater_than: {
                        operand1 = std::make_shared<GreaterThanOpNode>(
                            std::move(operand1),
                            std::move(operand2)
                        );
                    }
                    case TokenType::less_than: {
                        operand1 = std::make_shared<LessThanOpNode>(
                            std::move(operand1),
                            std::move(operand2)
                        );
                    }
                    default:
                        break;
                }
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
            auto operand1 = this->parseFunctionCallOrDotOperator();

            while (!tokenizer.isEmpty() && isIn(
                tokenizer.peekNextToken().type,
                TokenType::mul_op, TokenType::div_op
            )) {
                const auto op = tokenizer.getNextToken();
                auto operand2 = parseFunctionCallOrDotOperator();
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