#ifndef SOVIET_VALUETYPE_HPP
#define SOVIET_VALUETYPE_HPP

namespace soviet {
    enum class ValueType {
        NumberValue,
        BooleanValue,
        StringValue,
        UndefinedValue,
        FunctionValue,
        ObjectValue,
        ExplicitReturnValue,
        PrototypeValue,
        PrototypeObjectValue,
        ArrayValue
    };
}

#endif //SOVIET_VALUETYPE_HPP
