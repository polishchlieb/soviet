#pragma once
#include "Value.hpp"

namespace soviet {
	class BooleanValue : public Value {
	public:
		bool value;
		explicit BooleanValue(bool value);

		bool equals(const std::shared_ptr<Value>& other) override;
		std::shared_ptr<Value> clone() override;
		std::string dump() const override;

		std::shared_ptr<Value> negate();
	};
}