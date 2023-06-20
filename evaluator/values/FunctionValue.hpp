#ifndef SOVIET_FUNCTIONVALUE_HPP
#define SOVIET_FUNCTIONVALUE_HPP

#include "Value.hpp"
#include <functional>
#include <memory>
#include "../Scope.hpp"
#include "../../parser/nodes.hpp"

namespace soviet {
    class Evaluator;

    class FunctionValue : public Value {
    public:
        typedef std::function<std::shared_ptr<Value>(
            Evaluator& evaluator,
            std::vector<std::shared_ptr<Value>>& args
        )> RunFunction;
        typedef std::vector<std::shared_ptr<Scope>> DeclarationScope;

        RunFunction run;
        std::shared_ptr<PrototypeNode> prototype;
        bool isNative = false;

        FunctionValue(RunFunction run)
            : Value{ValueType::FunctionValue}, run(std::move(run)), isNative(true) {}

        FunctionValue(std::shared_ptr<PrototypeNode> prototype, DeclarationScope declarationScope)
            : Value{ValueType::FunctionValue}, prototype(std::move(prototype)),
              declarationScope(std::move(declarationScope)) {}

        DeclarationScope declarationScope;

        bool equals(const std::shared_ptr<Value>& other) {
            return other.get() == this;
        }

        std::shared_ptr<Value> clone() override {
            return std::shared_ptr<FunctionValue>{this};
        }

        std::string dump() const override {
            return "<function>";
        }
    };
}

#endif //SOVIET_FUNCTIONVALUE_HPP
