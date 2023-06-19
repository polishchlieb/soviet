#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../values/Value.hpp"

namespace soviet {
	class Evaluator;

	class Module {
	public:
		Module(Evaluator& evaluator) : evaluator(evaluator) {}

		std::string name;
		std::unordered_map<std::string, std::shared_ptr<Value>> variables;
	protected:
		Evaluator& evaluator;
	};
}