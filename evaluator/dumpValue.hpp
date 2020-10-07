#ifndef SOVIET_DUMPVALUE_HPP
#define SOVIET_DUMPVALUE_HPP

#include <string>
#include <memory>
#include "values/values.hpp"

namespace soviet {
    std::string dumpValue(const std::shared_ptr<Value>& value) {
        switch (value->type) {
            case ValueType::NumberValue: {
                const auto& v = value_cast<NumberValue>(value);
                return std::to_string(v->value);
            }
            case ValueType::BooleanValue: {
                const auto& v = value_cast<BooleanValue>(value);
                return v->value ? "true" : "false";
            }
            case ValueType::StringValue: {
                const auto& v = value_cast<StringValue>(value);
                return v->value;
            }
            case ValueType::ObjectValue: {
                const auto& v = value_cast<ObjectValue>(value);
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
            case ValueType::PrototypeObjectValue:
                return "[prototype object value]";
            default:
                return "<wtf>";
        }
    }
}

#endif //SOVIET_DUMPVALUE_HPP
