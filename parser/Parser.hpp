#pragma once
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
	};
}
