#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class MathModule : public Module {
	public:
		std::string name = "Math";
		MathModule(Evaluator& evaluator);

	private:
		static std::shared_ptr<Value> sin(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> cos(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> sqrt(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> pow(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> max(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> random(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
	};
}