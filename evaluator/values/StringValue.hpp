#ifndef SOVIET_STRINGVALUE_HPP
#define SOVIET_STRINGVALUE_HPP

#include <string>
#include "Value.hpp"
#include <algorithm>

namespace soviet {
    class StringValue : public Value {
    public:
        std::string value;

        explicit StringValue(std::string value);

        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        void append(const std::string& str);
        size_t length() const;
        void replaceAll(const std::string& substr, const std::string& replaceWith);
        void toUpperCase();
        void toLowerCase();
        std::string dump() const override;
    };
}

#endif //SOVIET_STRINGVALUE_HPP
