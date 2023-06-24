#include "ParseError.hpp"

namespace soviet {
	ParseError::ParseError(std::string value)
		: Error{ErrorType::Parse}, value(std::move(value))
	{}

	std::string ParseError::name() const noexcept {
		return "ParseError";
	}

	std::string ParseError::message() const noexcept {
		return this->value;
	}
}
