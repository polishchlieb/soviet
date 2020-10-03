#ifndef SOVIET_EXPLICITRETURNVALUE_HPP
#define SOVIET_EXPLICITRETURNVALUE_HPP

#include <memory>
#include "Value.hpp"

namespace soviet {
    struct ExplicitReturnValue : Value {
        std::shared_ptr<Value> value;

        explicit ExplicitReturnValue(std::shared_ptr<Value>&& value)
            : Value{ValueType::ExplicitReturnValue}, value(std::move(value)) {}
    };
}

#endif //SOVIET_EXPLICITRETURNVALUE_HPP
