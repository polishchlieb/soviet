#pragma once
#include "Module.hpp"

namespace soviet {
	class ThreadModule : public Module {
	public:
		std::string name = "Thread";
		ThreadModule(Evaluator& evaluator);

	private:
		static std::shared_ptr<Value> newthread(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> sleep(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args);
	};
}