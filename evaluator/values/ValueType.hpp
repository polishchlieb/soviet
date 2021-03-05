#ifndef SOVIET_VALUETYPE_HPP
#define SOVIET_VALUETYPE_HPP

namespace soviet {
    enum class ValueType {
        NumberValue,
        BooleanValue,
        StringValue,
        UndefinedValue,
        FunctionValue,
        ExplicitReturnValue,
        ArrayValue,
        MapValue
    };
}

#endif //SOVIET_VALUETYPE_HPP
