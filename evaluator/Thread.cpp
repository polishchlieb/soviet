#include "Thread.hpp"

namespace soviet {
	Thread::Thread(std::shared_ptr<FunctionValue> function) {
		evaluator = std::make_unique<Evaluator>(*this);

		thread = std::thread{ [this, function]() {
			std::vector<std::shared_ptr<Value>> args;
			evaluator->callFunction(function, args);
		} };
	}

	Thread::Thread() : evaluator{std::make_unique<Evaluator>(*this)} {}
}