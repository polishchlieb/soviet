#ifndef SOVIET_VALUETYPE_HPP
#define SOVIET_VALUETYPE_HPP

namespace soviet {
    enum class ValueType {
        NumberValue,
        BooleanValue,
        StringValue,
        UndefinedValue,
        FunctionValue,
        ObjectValue
    };
}

#endif //SOVIET_VALUETYPE_HPP
