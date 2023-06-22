#pragma once
#include "Module.hpp"
#include "../values/values.hpp"
#include <iostream>

namespace soviet {
	class IOModule : public Module {
	public:
        std::string name = "IO";
		IOModule(Evaluator& evaluator);

	private:
		static std::shared_ptr<Value> print(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> input(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args);
	};
}