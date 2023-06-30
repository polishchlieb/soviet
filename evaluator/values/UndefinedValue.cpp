#include "UndefinedValue.hpp"

namespace soviet {
	UndefinedValue::UndefinedValue() : Value{ValueType::UndefinedValue} {}

	bool UndefinedValue::equals(const std::shared_ptr<Value>& other) {
		return other->type == type;
	}

	std::shared_ptr<soviet::Value> UndefinedValue::clone() {
		return std::make_shared<UndefinedValue>();
	}

	std::string UndefinedValue::dump() const {
		return "<undefined>";
	}
}