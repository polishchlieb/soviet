#pragma once
#include <string>

namespace soviet {
    enum class ValueType {
        NumberValue,
        BooleanValue,
        StringValue,
        UndefinedValue,
        FunctionValue,
        ExplicitReturnValue,
        ArrayValue,
        MapValue,
        RangeValue,
        NullValue
    };

    std::string dumpValueType(const ValueType type);
}
