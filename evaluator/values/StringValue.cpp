#include "StringValue.hpp"
#include "../valueCast.hpp"

namespace soviet {
	StringValue::StringValue(std::string value)
		: Value{ValueType::StringValue}, value(std::move(value))
	{}

	bool StringValue::equals(const std::shared_ptr<Value>& other) {
		if (other->type != this->type)
			return false;
		const auto rightValue = valueCast<StringValue>(other)->value;
		return rightValue == value;
	}

	std::shared_ptr<soviet::Value> StringValue::clone() {
		return std::make_shared<StringValue>(value);
	}

	void StringValue::append(const std::string& str) {
		value += str;
	}

	size_t StringValue::length() const {
		return value.length();
	}

	void StringValue::replaceAll(const std::string& substr, const std::string& replaceWith) {
		size_t startPos = 0;
		while ((startPos = value.find(substr, startPos)) != std::string::npos) {
			value.replace(startPos, substr.length(), replaceWith);
			startPos += replaceWith.length();
		}
	}

	void StringValue::toUpperCase() {
		std::transform(value.begin(), value.end(), value.begin(), ::toupper);
	}

	void StringValue::toLowerCase() {
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
	}

	std::string StringValue::dump() const {
		return value;
	}
}