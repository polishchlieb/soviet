#ifndef SOVIET_PARSER_HPP
#define SOVIET_PARSER_HPP

#include "../tokenizer/Token.hpp"
#include <utility>
#include <vector>
#include "nodes.hpp"
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
	class Parser {
	public:
		explicit Parser(TokenList tokens)
			: tokens(std::move(tokens)) {}

		std::shared_ptr<Node> parse() {
			return this->parseExpression();
		}
		
		TokenList tokens;
	private:
		std::shared_ptr<Node> parseExpression() {
			return this->parseAssignment();
		}

		std::shared_ptr<Node> parsePrimary() {
			std::shared_ptr<Node> result;
			switch (tokens.peekNextToken().type) {
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
				case TokenType::negation:
					return parseNegation();
				case TokenType::open_square_bracket:
					return parseSquareBracketExpression();
				default: {
					const auto& token = tokens.peekNextToken();
					throw ParseError("unexpected token " + token.value + " on line " + std::to_string(token.line));
				}
			}
		}

		std::shared_ptr<Node> parseBracketExpression() {
			tokens.getNextToken(); // eat open bracket

			if (tokens.peekNextToken().type == TokenType::close_bracket) {
				tokens.getNextToken(); // eat close bracket

				const auto arrowTok = tokens.getNextToken(); // eat ->
				if (arrowTok.type != TokenType::arrow)
					throw ParseError("expected \"->\"");

				auto returnValue = this->parseExpression();

				return std::make_shared<PrototypeNode>(
					std::vector<std::shared_ptr<Node>>{},
					std::move(returnValue)
				);
			}

			auto operand = this->parseExpression();
			if (tokens.peekNextToken().type == TokenType::comma) {
				std::vector<std::shared_ptr<Node>> args{operand};
				while (tokens.peekNextToken().type == TokenType::comma) {
					tokens.getNextToken(); // eat comma
					args.push_back(this->parseExpression());
				}

				const auto closeBracket = tokens.getNextToken(); // eat )
				if (closeBracket.type != TokenType::close_bracket)
					throw ParseError("expected \")\"");

				const auto arrow = tokens.getNextToken(); // eat ->
				if (arrow.type != TokenType::arrow)
					throw ParseError("expected \"->\"");

				return std::make_shared<PrototypeNode>(
					std::move(args),
					this->parseExpression()
				);
			}

			const auto closeBracket = tokens.getNextToken(); // eat )
			if (closeBracket.type != TokenType::close_bracket)
				throw ParseError("expected \")\"");

			if (tokens.hasNextToken() && tokens.peekNextToken().type == TokenType::arrow) {
				tokens.getNextToken(); // eat ->

				return std::make_shared<PrototypeNode>(
					std::vector{operand},
					this->parseExpression()
				);
			}

			return operand;
		}

		std::shared_ptr<Node> parseSquareBracketExpression() {
			tokens.getNextToken(); // eat square bracket

			std::vector<std::shared_ptr<Node>> elements;
			if (tokens.peekNextToken().type == TokenType::close_square_bracket) {
				tokens.getNextToken(); // eat close square bracket
				return std::make_shared<ArrayNode>(std::move(elements));
			}

			while (true) {
				elements.push_back(this->parseExpression());

				if (tokens.peekNextToken().type == TokenType::close_square_bracket)
					break;
				if (tokens.getNextToken().type != TokenType::comma)
					throw ParseError("expected a comma");
			}

			tokens.getNextToken(); // eat close square bracket
			return std::make_shared<ArrayNode>(std::move(elements));
		}

		std::shared_ptr<Node> parseCurlyBracketExpression() {
			tokens.getNextToken(); // eat curly bracket

			std::vector<std::shared_ptr<Node>> nodes;
			while (tokens.peekNextToken().type != TokenType::close_curly_bracket)
				nodes.push_back(this->parseExpression());
			tokens.getNextToken(); // eat close curly bracket
			return std::make_shared<BlockNode>(std::move(nodes));
		}

		std::shared_ptr<Node> parseNumber() {
			auto result = std::make_shared<NumberNode>(
				std::stof(tokens.getNextToken().value)
			);

			return result;
		}

		std::shared_ptr<Node> parseIfStatement() {
			auto condition = this->parseExpression();

			auto body = this->parseExpression();

			if (tokens.hasNextToken()
				&& tokens.peekNextToken().type == TokenType::name
				&& tokens.peekNextToken().value == "else") { // "else"
				const auto elseTok = tokens.getNextToken(); // eat "else"
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

		std::shared_ptr<Node> parseWhileLoop() {
			auto condition = parseExpression();
			auto command = parseExpression();
			return std::make_shared<WhileLoopNode>(
				std::move(condition),
				std::move(command)
			);
		}

		std::shared_ptr<Node> parseString() {
			return std::make_shared<StringNode>(
				std::move(tokens.getNextToken().value)
			);
		}

		std::shared_ptr<Node> parseReturn() {
			return std::make_shared<ReturnNode>(
				this->parseExpression()
			);
		}

		std::shared_ptr<Node> parseName() {
			auto token = tokens.getNextToken();

			if (token.value == "if")
				return parseIfStatement();
			if (token.value == "return")
				return parseReturn();
			if (token.value == "true")
				return std::make_shared<BooleanNode>(true);
			if (token.value == "false")
				return std::make_shared<BooleanNode>(false);
			if (token.value == "while")
				return parseWhileLoop();

			auto node = std::make_shared<NameNode>(
				std::move(token.value)
			);

			if (!tokens.hasNextToken())
				return node;

			switch (tokens.peekNextToken().type) {
				case TokenType::arrow: {
					tokens.getNextToken(); // eat ->

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

			while (tokens.hasNextToken() && isIn(
				tokens.peekNextToken().type,
				TokenType::open_bracket, TokenType::dot
			)) {
				switch (tokens.getNextToken().type) {
					case TokenType::open_bracket: {
						std::vector<std::shared_ptr<Node>> args;
						if (tokens.peekNextToken().type != TokenType::close_bracket) {
							while (true) {
								auto arg = parseExpression();
								args.push_back(arg);

								if (tokens.peekNextToken().type == TokenType::close_bracket)
									break;
								if (tokens.peekNextToken().type != TokenType::comma)
									throw ParseError("function arguments must be separated by a comma");

								tokens.getNextToken(); // eat comma
							}
						}

						const auto closeBracket = tokens.getNextToken(); // eat )
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

						result = std::make_shared<BinOpNode>(
							BinOpType::Dot,
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

		std::shared_ptr<Node> parseNegation() {
			tokens.getNextToken(); // eat !
			return std::make_shared<NegationNode>(parseExpression());
		}

		std::shared_ptr<Node> parseAssignment() {
			auto operand1 = parseComparison();
			while (tokens.hasNextToken()
			  && tokens.peekNextToken().type == TokenType::equals_op) {
				tokens.getNextToken(); // eat =
				auto operand2 = parseComparison();
				operand1 = std::make_shared<BinOpNode>(
					BinOpType::Equals,
					std::move(operand1),
					std::move(operand2)
				);
			}
			return operand1;
		}

		std::shared_ptr<Node> parseComparison() {
			auto operand1 = this->parseAdditive();
			while (tokens.hasNextToken() && isIn(
				tokens.peekNextToken().type,
				TokenType::double_equals_op, TokenType::greater_than,
				TokenType::less_than_op, TokenType::greater_than_or_equal_op,
				TokenType::less_than_or_equal_op
			)) {
				const auto op = tokens.getNextToken();
				auto operand2 = parseAdditive();
				switch (op.type) {
					case TokenType::double_equals_op: {
						operand1 = std::make_shared<BinOpNode>(
							BinOpType::DoubleEquals,
							std::move(operand1),
							std::move(operand2)
						);
						break;
					}
					case TokenType::greater_than: {
						operand1 = std::make_shared<BinOpNode>(
							BinOpType::GreaterThan,
							std::move(operand1),
							std::move(operand2)
						);
						break;
					}
					case TokenType::less_than_op: {
						operand1 = std::make_shared<BinOpNode>(
							BinOpType::LessThan,
							std::move(operand1),
							std::move(operand2)
						);
						break;
					}
					case TokenType::greater_than_or_equal_op:  {
						operand1 = std::make_shared<BinOpNode>(
							BinOpType::GreaterThanOrEqual,
							std::move(operand1),
							std::move(operand2)
						);
						break;
					}
					case TokenType::less_than_or_equal_op: {
						operand1 = std::make_shared<BinOpNode>(
							BinOpType::LessThanOrEqual,
							std::move(operand1),
							std::move(operand2)
						);
						break;
					}
					default:
						break;
				}
			}
			return operand1;
		}

		std::shared_ptr<Node> parseAdditive() {
			auto operand1 = this->parseMultiplicative();
			while (tokens.hasNextToken() && isIn(
				tokens.peekNextToken().type,
				TokenType::add_op, TokenType::sub_op
			)) {
				const auto op = tokens.getNextToken();
				auto operand2 = parseMultiplicative();
				if (op.type == TokenType::add_op) {
					operand1 = std::make_shared<BinOpNode>(
						BinOpType::Add,
						std::move(operand1),
						std::move(operand2)
					);
				} else {
					operand1 = std::make_shared<BinOpNode>(
						BinOpType::Subtract,
						std::move(operand1),
						std::move(operand2)
					);
				}
			}
			return operand1;
		}

		std::shared_ptr<Node> parseMultiplicative() {
			auto operand1 = this->parseFunctionCallOrDotOperator();

			while (tokens.hasNextToken() && isIn(
				tokens.peekNextToken().type,
				TokenType::mul_op, TokenType::div_op
			)) {
				const auto op = tokens.getNextToken();
				auto operand2 = parseFunctionCallOrDotOperator();
				if (op.type == TokenType::mul_op) {
					operand1 = std::make_shared<BinOpNode>(
						BinOpType::Multiply,
						std::move(operand1),
						std::move(operand2)
					);
				} else {
					operand1 = std::make_shared<BinOpNode>(
						BinOpType::Divide,
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