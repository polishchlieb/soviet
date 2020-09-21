#ifndef SOVIET_FUNCTIONVALUE_HPP
#define SOVIET_FUNCTIONVALUE_HPP

#include "Value.hpp"
#include <functional>
#include <memory>

namespace soviet {
    struct FunctionValue : Value {
        std::function<std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>>&)> run;
        explicit FunctionValue(
            std::function<std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>>&)>&& run
        )
            : Value{ValueType::FunctionValue}, run(std::move(run)) {}
    };
}

#endif //SOVIET_FUNCTIONVALUE_HPP
