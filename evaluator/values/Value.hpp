#pragma once
#include <memory>
#include "ValueType.hpp"

namespace soviet {
    class Value {
    public:
        ValueType type;

        explicit Value(ValueType type) : type(type) {}

        virtual bool equals(const std::shared_ptr<Value>&) = 0;
        virtual std::shared_ptr<Value> clone() = 0;
        virtual std::string dump() const = 0;
    };
}