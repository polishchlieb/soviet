#pragma once
#include "Module.hpp"

namespace soviet {
	class ThreadModule : public Module {
	public:
		std::string name = "Thread";

		ThreadModule(Evaluator& evaluator);
	};
}