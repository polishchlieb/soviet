#pragma once
#include "NumberValue.hpp"

namespace soviet {
	class RangeValue : public Value {
	public:
		RangeValue(size_t from, size_t to)
			: Value{ValueType::RangeValue}, from(from), to(to) {}

		RangeValue(size_t to)
			: Value{ValueType::RangeValue}, from(0), to(to) {}

		std::string dump() const override {
			return "range(" + std::to_string(from) + ", " + std::to_string(to) + ")";
		}

		std::shared_ptr<Value> clone() override {
			return std::make_shared<RangeValue>(from, to);
		}

		bool equals(const std::shared_ptr<Value>& other) override {
			if (this->type != other->type)
				return false;
			const auto otherValue = valueCast<RangeValue>(other);
			return this->from == otherValue->from && this->to == otherValue->to;
		}

		const size_t from, to;
	};
}