#pragma once
#include <memory>
#include "Value.hpp"

namespace soviet {
    class NumberValue : public Value {
    public:
        float value;
        explicit NumberValue(float value);

        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        std::string dump() const override;

        bool isInt() const;
    };
}