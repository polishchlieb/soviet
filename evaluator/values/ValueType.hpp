#ifndef SOVIET_VALUETYPE_HPP
#define SOVIET_VALUETYPE_HPP

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
        RangeValue
    };

    static std::string dumpValueType(const ValueType type) {
        switch (type) {
            case ValueType::ArrayValue: return "array";
            case ValueType::BooleanValue: return "boolean";
            case ValueType::ExplicitReturnValue: return "explicit_return_value";
            case ValueType::FunctionValue: return "function";
            case ValueType::NumberValue: return "number";
            case ValueType::StringValue: return "string";
            case ValueType::UndefinedValue: return "undefined";
            case ValueType::MapValue: return "map";
            case ValueType::RangeValue: return "range";
        }
        return "wtf";
    }
}

#endif //SOVIET_VALUETYPE_HPP
