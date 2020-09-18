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
            case ValueType::UndefinedValue:
                return "";
        }
    }
}

#endif //SOVIET_DUMPVALUE_HPP
