#include "NullValue.hpp"

namespace soviet {
	NullValue::NullValue() : Value{ValueType::NullValue} {}

	bool NullValue::equals(const std::shared_ptr<Value>& other) {
		return other->type == type;
	}

	std::shared_ptr<Value> NullValue::clone() {
		return std::make_shared<NullValue>();
	}

	std::string NullValue::dump() const {
		return "null";
	}
}