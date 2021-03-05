#ifndef SOVIET_STRINGVALUE_HPP
#define SOVIET_STRINGVALUE_HPP

#include <string>
#include "Value.hpp"

namespace soviet {
    class StringValue : public Value {
    public:
        std::string value;
        explicit StringValue(std::string value)
            : Value{ValueType::StringValue}, value(std::move(value)) {}

        bool equals(const std::shared_ptr<Value>& other) override {
            if (other->type != this->type)
                return false;
            const auto rightValue = valueCast<StringValue>(other)->value;
            return rightValue == value;
        }
    };
}

#endif //SOVIET_STRINGVALUE_HPP
