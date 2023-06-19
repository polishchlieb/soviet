#include "ThreadModule.hpp"
#include "../Thread.hpp"
#include "../evaluator.hpp"
#include <sstream>

namespace soviet {
	ThreadModule::ThreadModule(Evaluator& evaluator) : Module{evaluator} {
		variables.insert({
			"new",
			std::make_shared<FunctionValue>(
				[&evaluator](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
					auto func = valueCast<FunctionValue>(args[0]);
					auto& thread = evaluator.thread.childThreads.emplace_back(func);

					std::stringstream s;
					s << thread.thread.get_id();

					return std::make_shared<StringValue>(s.str());
				}
			)
		});

		variables.insert({
			"sleep",
			std::make_shared<FunctionValue>(
				[&evaluator](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
					auto interval = valueCast<NumberValue>(args[0]);
					std::this_thread::sleep_for(std::chrono::milliseconds((int) interval->value));
					return std::make_shared<UndefinedValue>();
				}
			)
		});
	}
}