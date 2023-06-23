#include "RangeValue.hpp"
#include "../valueCast.hpp"

namespace soviet {
	RangeValue::RangeValue(size_t from, size_t to)
		: Value{ValueType::RangeValue}, from(from), to(to)
	{}

	RangeValue::RangeValue(size_t to)
		: Value{ValueType::RangeValue}, from(0), to(to)
	{}

	std::string RangeValue::dump() const {
		return "range(" + std::to_string(from) + ", " + std::to_string(to) + ")";
	}

	std::shared_ptr<soviet::Value> RangeValue::clone() {
		return std::make_shared<RangeValue>(from, to);
	}

	bool RangeValue::equals(const std::shared_ptr<Value>& other) {
		if (this->type != other->type)
			return false;
		const auto otherValue = valueCast<RangeValue>(other);
		return this->from == otherValue->from && this->to == otherValue->to;
	}
}