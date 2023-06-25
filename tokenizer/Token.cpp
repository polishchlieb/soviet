#include "Token.hpp"
#include <utility>

namespace soviet {
	Token::Token(Token&& other) noexcept
		: type(std::exchange(other.type, TokenType::undefined)),
		  value(std::move(other.value)),
		  line(std::exchange(other.line, UNDEFINED_LINE))
	{}

	Token::Token(TokenType type, std::string&& value, unsigned int line)
		: type(type), value(std::move(value)), line(line)
	{}

	bool Token::isEmpty() const {
		return this->type == TokenType::none;
	}

	void Token::clear(unsigned int line) {
		this->type = TokenType::none;
		this->value = "";
		this->line = line;
	}
}