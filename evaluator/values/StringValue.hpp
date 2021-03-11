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

        std::shared_ptr<Value> clone() override {
            return std::make_shared<StringValue>(value);
        }

        void append(const std::string& str) {
            value += str;
        }

        size_t length() const {
            return value.length();
        }

        void replaceAll(const std::string& substr, const std::string& replaceWith) {
            size_t startPos = 0;
            while ((startPos = value.find(substr, startPos)) != std::string::npos) {
                value.replace(startPos, substr.length(), replaceWith);
                startPos += replaceWith.length();
            }
        }

        void toUpperCase() {
            std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        }
    
        void toLowerCase() {
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        }
    };
}

#endif //SOVIET_STRINGVALUE_HPP
