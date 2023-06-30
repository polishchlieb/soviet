#pragma once
#include "Value.hpp"

namespace soviet {
	class UndefinedValue : public Value {
	public:
		UndefinedValue();

		bool equals(const std::shared_ptr<Value>& other) override;
		std::shared_ptr<Value> clone() override;
		std::string dump() const override;
	};
}