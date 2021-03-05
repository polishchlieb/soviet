#ifndef SOVIET_EXPLICITRETURNVALUE_HPP
#define SOVIET_EXPLICITRETURNVALUE_HPP

#include <memory>
#include "Value.hpp"

namespace soviet {
    class ExplicitReturnValue : public Value {
    public:
        std::shared_ptr<Value> value;

        explicit ExplicitReturnValue(std::shared_ptr<Value>&& value)
            : Value{ValueType::ExplicitReturnValue}, value(std::move(value)) {}

        bool equals(const std::shared_ptr<Value>& other) {
            return false;
        }
    };
}

#endif //SOVIET_EXPLICITRETURNVALUE_HPP
