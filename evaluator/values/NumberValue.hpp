#ifndef SOVIET_NUMBERVALUE_HPP
#define SOVIET_NUMBERVALUE_HPP

#include "Value.hpp"

namespace soviet {
    struct NumberValue : Value {
        float value;
        explicit NumberValue(float value) : Value{ValueType::NumberValue}, value(value) {}
    };
}

#endif //SOVIET_NUMBERVALUE_HPP
