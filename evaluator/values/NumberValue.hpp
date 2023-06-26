#pragma once
#include <memory>
#include "Value.hpp"
#include "BooleanValue.hpp"

namespace soviet {
    class NumberValue : public Value {
    public:
        float value;
        explicit NumberValue(float value);

        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        std::string dump() const override;

        bool isInt() const;

        std::shared_ptr<Value> add(const std::shared_ptr<Value>& right);
        std::shared_ptr<Value> multiply(const std::shared_ptr<Value>& right);
        std::shared_ptr<Value> subtract(const std::shared_ptr<Value>& right);
        std::shared_ptr<Value> divide(const std::shared_ptr<Value>& right);
        bool greaterThan(const std::shared_ptr<Value>& right);
    };
}