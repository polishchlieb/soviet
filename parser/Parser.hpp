#pragma once
#include "../tokenizer/TokenList.hpp"
#include "nodes.hpp"
#include <memory>

namespace soviet {
	/* Operator precedence:
		1. assignment
		2. logical or
		3. logical and
		4. comparison (==, >, >=, <, <=, !=)
		5. additive (+, -)
		6. multiplicative (*, /)
		7. pipe
		8. function call, dot op, array at op
		9. primary (leaf nodes, bracket expression) */
	class Parser {
	public:
		explicit Parser(TokenList tokens);
		std::shared_ptr<Node> parse();
		TokenList tokens;
	private:
		std::shared_ptr<Node> parseExpression();
		std::shared_ptr<Node> parsePrimary();
		std::shared_ptr<Node> parseBracketExpression();
		std::shared_ptr<Node> parseSquareBracketExpression();
		std::shared_ptr<Node> parseCurlyBracketExpression();
		std::shared_ptr<Node> parseNumber();
		std::shared_ptr<Node> parseIfStatement();
		std::shared_ptr<Node> parseWhileLoop();
		std::shared_ptr<Node> parseForLoop();
		std::shared_ptr<Node> parseModuleDefinition();
		std::shared_ptr<Node> parseString();
		std::shared_ptr<Node> parseReturn();
		std::shared_ptr<Node> parseName();
		std::shared_ptr<Node> parseFunctionCallOrDotOperator();
		std::shared_ptr<Node> parseNegation();
		std::shared_ptr<Node> parseImport();
		std::shared_ptr<Node> parseAssignment();
		std::shared_ptr<Node> parseComparison();
		std::shared_ptr<Node> parseAdditive();
		std::shared_ptr<Node> parsePipe();
		std::shared_ptr<Node> parseMultiplicative();
		std::shared_ptr<Node> parseEmptyPipeOp();
		std::shared_ptr<Node> parseAnd();
		std::shared_ptr<Node> parseOr();
	};
}
