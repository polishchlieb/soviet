#ifndef SOVIET_VALUE_HPP
#define SOVIET_VALUE_HPP

#include "ValueType.hpp"

namespace soviet {
    struct Value {
        ValueType type;

        explicit Value(ValueType type) : type(type) {}
    };
}

#endif //SOVIET_VALUE_HPP
