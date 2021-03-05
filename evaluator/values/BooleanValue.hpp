#ifndef SOVIET_BOOLEANVALUE_HPP
#define SOVIET_BOOLEANVALUE_HPP

#include "Value.hpp"

namespace soviet {
    class BooleanValue : public Value {
    public:
        bool value;
        explicit BooleanValue(bool value)
            : Value{ValueType::BooleanValue}, value(value) {}

        bool equals(const std::shared_ptr<Value>& other) {
            if (other->type != this->type)
                return false;
            const auto otherValue = valueCast<BooleanValue>(other);
            return otherValue->value == value;
        }
    };
}

#endif //SOVIET_BOOLEANVALUE_HPP
