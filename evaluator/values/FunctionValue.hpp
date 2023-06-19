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

        explicit FunctionValue(RunFunction run)
            : Value{ ValueType::FunctionValue }, run(std::move(run)), isNative(true) {}

        /*explicit FunctionValue(RunFunction run, DeclarationScope declarationScope)
            : Value{ValueType::FunctionValue}, run(std::move(run)),
            declarationScope(std::move(declarationScope)) {} */

        explicit FunctionValue(std::shared_ptr<PrototypeNode> prototype, DeclarationScope declarationScope)
            : Value{ValueType::FunctionValue}, prototype(std::move(prototype)),
              declarationScope(std::move(declarationScope)) {}

        DeclarationScope declarationScope;

        bool equals(const std::shared_ptr<Value>& other) {
            return false;
        }

        /*std::shared_ptr<Value> clone() override {
            return std::make_shared<FunctionValue>(run);
        }*/
        std::shared_ptr<Value> clone() override {
            return nullptr;
        }

        std::string dump() const override {
            return "<function>";
        }
    };
}

#endif //SOVIET_FUNCTIONVALUE_HPP
