#include "NumberValue.hpp"
#include "../valueCast.hpp"

namespace soviet {
	NumberValue::NumberValue(float value)
		: Value{ValueType::NumberValue}, value(value) {}

	bool NumberValue::equals(const std::shared_ptr<Value>& other) {
		if (other->type != this->type)
			return false;
		const auto otherValue = valueCast<NumberValue>(other);
		return otherValue->value == this->value;
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
}