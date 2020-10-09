#ifndef SOVIET_DUMPVALUE_HPP
#define SOVIET_DUMPVALUE_HPP

#include <string>
#include <memory>
#include "values/values.hpp"

namespace soviet {
    std::string dumpValueType(const ValueType type) {
        switch (type) {
            case ValueType::ArrayValue: return "array";
            case ValueType::BooleanValue: return "boolean";
            case ValueType::ExplicitReturnValue: return "explicit_return_value";
            case ValueType::FunctionValue: return "function";
            case ValueType::NumberValue: return "number";
            case ValueType::ObjectValue: return "object";
            case ValueType::PrototypeValue: return "prototype";
            case ValueType::PrototypeObjectValue: return "prototype_object";
            case ValueType::StringValue: return "string";
            case ValueType::UndefinedValue: return "undefined";
        }
        return "wtf";
    }

    std::string dumpValue(const std::shared_ptr<Value>& value) {
        switch (value->type) {
            case ValueType::NumberValue: {
                const auto v = valueCast<NumberValue>(value);
                return std::to_string(v->value);
            }
            case ValueType::BooleanValue: {
                const auto v = valueCast<BooleanValue>(value);
                return v->value ? "true" : "false";
            }
            case ValueType::StringValue: {
                const auto v = valueCast<StringValue>(value);
                return v->value;
            }
            case ValueType::ObjectValue: {
                const auto v = valueCast<ObjectValue>(value);
                std::string result = "object(";
                for (auto [key, value] : v->getProperties())
                    result += key + " => " + dumpValue(value) + ", ";
                return result.substr(0, result.length() - 2) + ")";
            }
            case ValueType::FunctionValue:
                return "[function]";
            case ValueType::UndefinedValue:
                return "[undefined]";
            case ValueType::ExplicitReturnValue:
                return "<explicit return value>";
            case ValueType::PrototypeObjectValue: {
                const auto v = valueCast<PrototypeObjectValue>(value);
                return "prototype<" + dumpValueType(v->object->type) + ">";
            }
            default:
                return "<wtf>";
        }
    }
}

#endif //SOVIET_DUMPVALUE_HPP
