#pragma once
#include "../util/Error.hpp"

namespace soviet {
	class NoTokensError : public Error {
	public:
		NoTokensError();

		std::string message() const override;
		std::string name() const override;
	};
}