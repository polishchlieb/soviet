#include "BooleanValue.hpp"
#include "../valueCast.hpp"

namespace soviet {
	BooleanValue::BooleanValue(bool value)
		: Value{ValueType::BooleanValue}, value(value)
	{}

	bool BooleanValue::equals(const std::shared_ptr<Value>& other) {
		if (other->type != this->type)
			return false;
		const auto otherValue = valueCast<BooleanValue>(other);
		return otherValue->value == value;
	}

	std::shared_ptr<soviet::Value> BooleanValue::clone() {
		return std::make_shared<BooleanValue>(value);
	}

	std::string BooleanValue::dump() const {
		return value ? "true" : "false";
	}

	std::shared_ptr<Value> BooleanValue::negate() {
		return std::make_shared<BooleanValue>(!value);
	}
}