#ifndef SOVIET_BOOLEANVALUE_HPP
#define SOVIET_BOOLEANVALUE_HPP

#include "Value.hpp"

namespace soviet {
    struct BooleanValue : Value {
        bool value;
        explicit BooleanValue(bool value) : Value{ValueType::BooleanValue}, value(value) {}
    };
}

#endif //SOVIET_BOOLEANVALUE_HPP
