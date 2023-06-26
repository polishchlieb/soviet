#include "StringValue.hpp"
#include "../valueCast.hpp"
#include <algorithm>
#include "NumberValue.hpp"
#include "../../util/util.hpp"
#include "../EvaluateError.hpp"

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

	std::shared_ptr<soviet::Value> StringValue::add(const std::shared_ptr<Value>& right) {
		switch (right->type) {
			case ValueType::NumberValue: {
				const auto rightValue = valueCast<NumberValue>(right)->value;
				return std::make_shared<StringValue>(
					value + std::to_string(rightValue)
				);
			}
			case ValueType::StringValue: {
				const auto& rightValue = valueCast<StringValue>(right)->value;
				return std::make_shared<StringValue>(value + rightValue);
			}
		}

		throw EvaluateError{"unknown operands"};
	}

	std::shared_ptr<soviet::Value> StringValue::multiply(const std::shared_ptr<Value>& right) {
		if (right->type != ValueType::NumberValue)
			throw EvaluateError{"string can be multiplied only by a number"};

		return std::make_shared<StringValue>(times(
			value,
			static_cast<unsigned int>(
				valueCast<NumberValue>(right)->value
			)
		));
	}
}