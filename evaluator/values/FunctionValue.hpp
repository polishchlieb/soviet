#pragma once
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
        DeclarationScope declarationScope;

		FunctionValue(RunFunction run);
		FunctionValue(std::shared_ptr<PrototypeNode> prototype, DeclarationScope declarationScope);

		bool equals(const std::shared_ptr<Value>& other) override;
		std::shared_ptr<Value> clone() override;
		std::string dump() const override;
    };
}
