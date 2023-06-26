#include "NumberValue.hpp"
#include "../valueCast.hpp"
#include "StringValue.hpp"
#include "../../util/util.hpp"
#include "../EvaluateError.hpp"

namespace soviet {
	NumberValue::NumberValue(float value)
		: Value{ValueType::NumberValue}, value(value) {}

	bool NumberValue::equals(const std::shared_ptr<Value>& right) {
		if (right->type != this->type)
			return false;
		const auto rightValue = valueCast<NumberValue>(right);
		return std::fabs(rightValue->value - this->value) <= DBL_EPSILON;
	}

	std::shared_ptr<soviet::Value> NumberValue::clone() {
		return std::make_shared<NumberValue>(value);
	}

	std::string NumberValue::dump() const {
		return std::to_string(value);
	}

	bool NumberValue::isInt() const {
		return ceilf(value) == value;
	}

	std::shared_ptr<soviet::Value> NumberValue::add(const std::shared_ptr<Value>& right) {
		switch (right->type) {
			case ValueType::NumberValue: {
				const auto rightValue = valueCast<NumberValue>(right)->value;
				return std::make_shared<NumberValue>(value + rightValue);
			}
			case ValueType::StringValue: {
				const auto& rightValue = valueCast<StringValue>(right)->value;
				return std::make_shared<StringValue>(
					std::to_string(value) + rightValue
				);
			}
		}

		throw EvaluateError{"unknown operands"};
	}

	std::shared_ptr<soviet::Value> NumberValue::multiply(const std::shared_ptr<Value>& right) {
		switch (right->type) {
			case ValueType::NumberValue:
				return std::make_shared<NumberValue>(
					value * valueCast<NumberValue>(right)->value
				);
			case ValueType::StringValue:
				return std::make_shared<StringValue>(times(
					valueCast<StringValue>(right)->value,
					static_cast<unsigned int>(value)
				));
		}

		throw EvaluateError{"unknown operands"};
	}

	std::shared_ptr<soviet::Value> NumberValue::subtract(const std::shared_ptr<Value>& right) {
		if (right->type != ValueType::NumberValue)
			throw EvaluateError{"subtraction operands have to be number values"};
		return std::make_shared<NumberValue>(value - valueCast<NumberValue>(right)->value);
	}

	std::shared_ptr<soviet::Value> NumberValue::divide(const std::shared_ptr<Value>& right) {
		if (right->type != ValueType::NumberValue)
			throw EvaluateError{"division operands have to be number values"};

		const auto rightValue = valueCast<NumberValue>(right)->value;
		if (std::fabs(rightValue) < DBL_EPSILON)
			throw EvaluateError{"division by 0"};
		
		return std::make_shared<NumberValue>(value / rightValue);
	}

	bool NumberValue::greaterThan(const std::shared_ptr<Value>& right) {
		if (right->type != ValueType::NumberValue)
			throw EvaluateError{"unknown operands"};

		const auto rightValue = valueCast<NumberValue>(right)->value;
		return value - rightValue > DBL_EPSILON;
	}
}