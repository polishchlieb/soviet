#ifndef SOVIET_STRINGVALUE_HPP
#define SOVIET_STRINGVALUE_HPP

#include <string>
#include "Value.hpp"

namespace soviet {
    struct StringValue : Value {
        std::string value;
        explicit StringValue(std::string&& value)
            : Value{ValueType::StringValue}, value(std::move(value)) {}
    };
}

#endif //SOVIET_STRINGVALUE_HPP
