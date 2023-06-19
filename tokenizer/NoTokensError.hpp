#pragma once
#include "../util/Error.hpp"

namespace soviet {
	class NoTokensError : public Error {
	public:
		NoTokensError() : Error{ErrorType::NoTokens} {}

		std::string message() const override {
			return "";
		}

		std::string name() const override {
			return "NoTokensError";
		}
	};
}