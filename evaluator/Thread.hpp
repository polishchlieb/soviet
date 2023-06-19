#pragma once
#include "evaluator.hpp"
#include <thread>

namespace soviet {
	class Thread {
	public:
		Thread(std::shared_ptr<FunctionValue> function);
		Thread();

		std::unique_ptr<Evaluator> evaluator;
		std::thread thread;
		std::vector<Thread> childThreads;
	};
}