#include "FileReadError.hpp"

namespace soviet {
	FileReadError::FileReadError(std::string value)
		: value(std::move(value)) {}

	const char* FileReadError::name() const noexcept {
		return "FileReadError";
	}

	std::string FileReadError::message() const noexcept {
		return this->value;
	}
}