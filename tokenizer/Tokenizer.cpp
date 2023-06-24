#include "Tokenizer.hpp"

namespace soviet {
	void Tokenizer::tokenize(const std::string& line) {
		previous.clear(++lineNumber);

		for (const char c : line) {
			parseChar(c);
		}

		if (!previous.isEmpty()) {
			if (previous.type == TokenType::string)
				throw ParseError(
					"expected string ending on line " + std::to_string(previous.line)
				);
			if (previous.type == TokenType::comment)
				return;

			tokens.emplace(std::move(previous));
		}
	}

	soviet::TokenList Tokenizer::getTokens() {
		return TokenList{tokens};
	}

	void Tokenizer::parseChar(const char c) {
		switch (previous.type) {
			case TokenType::none: parseNone(c); break;
			case TokenType::string: parseString(c); break;
			case TokenType::number: parseNumber(c); break;
			case TokenType::name: parseName(c); break;
			case TokenType::sub_op: parseSubOp(c); break;
			case TokenType::div_op: parseDivOp(c); break;
			case TokenType::equals_op: parseEqualsOp(c); break;
			case TokenType::greater_than: parseGreaterThan(c); break;
			case TokenType::comment: parseComment(c); break;
			case TokenType::negation: parseNegation(c); break;
			case TokenType::less_than_op: parseLessThan(c); break;
			case TokenType::or_op: parseOr(c); break;
			case TokenType::greater_than_or_equal_op:
			case TokenType::less_than_or_equal_op:
			case TokenType::open_square_bracket:
			case TokenType::close_square_bracket:
			case TokenType::colon:
			case TokenType::dot:
			case TokenType::not_equals_op:
			case TokenType::add_op:
			case TokenType::mul_op:
			case TokenType::open_bracket:
			case TokenType::close_bracket:
			case TokenType::open_curly_bracket:
			case TokenType::close_curly_bracket:
			case TokenType::double_equals_op:
			case TokenType::comma:
			case TokenType::arrow:
			case TokenType::pipe_op:
			case TokenType::and_op:
				parseFixed(c);
				break;
			default:
				throw ParseError("unexpected token on line " + std::to_string(lineNumber));
		}
	}

	TokenType Tokenizer::getType(const char c) {
		if (isdigit(c)) return TokenType::number;
		if (isalpha(c) || c == '_') return TokenType::name;
		switch (c) {
			case ' ': return TokenType::none;
			case '\'': case '"': return TokenType::string;
			case '+': return TokenType::add_op;
			case '-': return TokenType::sub_op;
			case '*': return TokenType::mul_op;
			case '/': return TokenType::div_op;
			case '(': return TokenType::open_bracket;
			case ')': return TokenType::close_bracket;
			case '=': return TokenType::equals_op;
			case ',': return TokenType::comma;
			case '>': return TokenType::greater_than;
			case '<': return TokenType::less_than_op;
			case '{': return TokenType::open_curly_bracket;
			case '}': return TokenType::close_curly_bracket;
			case '.': return TokenType::dot;
			case '!': return TokenType::negation;
			case '[': return TokenType::open_square_bracket;
			case ']': return TokenType::close_square_bracket;
			case ':': return TokenType::colon;
			case '|': return TokenType::or_op;
			case '&': return TokenType::and_op;
		}
		return TokenType::unknown;
	}

	void Tokenizer::parseNone(const char c) {
		const auto type = getType(c);

		if (type == TokenType::string) {
			previous.type = type;
		} else if (type != TokenType::none) {
			previous.type = type;
			previous.value += c;
		}
	}

	void Tokenizer::parseString(const char c) {
		if (c == '\'' || c == '"') {
			tokens.emplace(std::move(previous));
			previous.clear(lineNumber);
			return;
		}

		previous.value += c;
	}

	void Tokenizer::parseNumber(const char c) {
		if (isIn(getType(c), TokenType::number, TokenType::dot)) {
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseName(const char c) {
		if (isIn(getType(c), TokenType::name, TokenType::number)) {
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseSubOp(const char c) {
		if (getType(c) == TokenType::greater_than) {
			previous.type = TokenType::arrow;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseDivOp(const char c) {
		const auto type = getType(c);
		if (type == TokenType::div_op) {
			previous.type = TokenType::comment;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseEqualsOp(const char c) {
		const auto type = getType(c);
		if (type == TokenType::equals_op) {
			previous.type = TokenType::double_equals_op;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseGreaterThan(const char c) {
		const auto type = getType(c);
		if (type == TokenType::equals_op) {
			previous.type = TokenType::greater_than_or_equal_op;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseLessThan(const char c) {
		const auto type = getType(c);
		if (type == TokenType::equals_op) {
			previous.type = TokenType::less_than_or_equal_op;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseComment(const char c) {}

	void Tokenizer::parseNegation(const char c) {
		const auto type = getType(c);
		if (type == TokenType::equals_op) {
			previous.type = TokenType::not_equals_op;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseOr(const char c) {
		const auto type = getType(c);
		if (type == TokenType::greater_than) {
			previous.type = TokenType::pipe_op;
			previous.value += c;
			return;
		}

		parseFixed(c);
	}

	void Tokenizer::parseFixed(const char c) {
		tokens.emplace(std::move(previous));
		previous.clear(lineNumber);
		parseChar(c);
	}
}