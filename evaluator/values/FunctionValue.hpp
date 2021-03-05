#ifndef SOVIET_FUNCTIONVALUE_HPP
#define SOVIET_FUNCTIONVALUE_HPP

#include "Value.hpp"
#include <functional>
#include <memory>

namespace soviet {
    class FunctionValue : public Value {
    public:
        std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>&)> run;
        explicit FunctionValue(
            std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>&)> run
        )
            : Value{ValueType::FunctionValue}, run(std::move(run)) {}

        bool equals(const std::shared_ptr<Value>& other) {
            return false;
        }
    };
}

#endif //SOVIET_FUNCTIONVALUE_HPP
