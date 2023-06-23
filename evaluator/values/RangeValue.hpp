#pragma once
#include "NumberValue.hpp"

namespace soviet {
	class RangeValue : public Value {
	public:
		RangeValue(size_t from, size_t to);
		RangeValue(size_t to);

		std::string dump() const override;
		std::shared_ptr<Value> clone() override;
		bool equals(const std::shared_ptr<Value>& other) override;

		const size_t from, to;
	};
}