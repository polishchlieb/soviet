#include "FunctionValue.hpp"

namespace soviet {
	FunctionValue::FunctionValue(RunFunction run)
		: Value{ValueType::FunctionValue}, run(std::move(run)), isNative(true)
	{}

	FunctionValue::FunctionValue(std::shared_ptr<PrototypeNode> prototype, DeclarationScope declarationScope)
		: Value{ValueType::FunctionValue}, prototype(std::move(prototype)),
		  declarationScope(std::move(declarationScope))
	{}

	bool FunctionValue::equals(const std::shared_ptr<Value>& other) {
		return other.get() == this;
	}

	std::shared_ptr<soviet::Value> FunctionValue::clone() {
		return std::shared_ptr<FunctionValue>{this};
	}

	std::string FunctionValue::dump() const {
		return "<function>";
	}
}