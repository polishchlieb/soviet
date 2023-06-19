#ifndef SOVIET_NUMBERVALUE_HPP
#define SOVIET_NUMBERVALUE_HPP

#include "Value.hpp"

namespace soviet {
    class NumberValue : public Value {
    public:
        float value;
        explicit NumberValue(float value)
            : Value{ValueType::NumberValue}, value(value) {}

        bool equals(const std::shared_ptr<Value>& other) override {
            if (other->type != this->type)
                return false;
            const auto otherValue = valueCast<NumberValue>(other);
            return otherValue->value == this->value;
        }

        std::shared_ptr<Value> clone() override {
            return std::make_shared<NumberValue>(value);
        }

        std::string dump() const override {
            return std::to_string(value);
        }
    };
}

#endif //SOVIET_NUMBERVALUE_HPP
