#ifndef SOVIET_VALUE_HPP
#define SOVIET_VALUE_HPP

#include "ValueType.hpp"

namespace soviet {
    class Value {
    public:
        ValueType type;

        explicit Value(ValueType type) : type(type) {}

        virtual bool equals(const std::shared_ptr<Value>&) = 0;
    };

    class UndefinedValue : public Value {
    public:
        ValueType type;

        UndefinedValue() : Value{ValueType::UndefinedValue} {}

        bool equals(const std::shared_ptr<Value>& other) {
            return other->type == type;
        }
    };
}

#endif //SOVIET_VALUE_HPP
