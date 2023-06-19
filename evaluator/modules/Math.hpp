#pragma once
#include "Module.hpp"
#include "../values/values.hpp"
#include <random>

namespace soviet {
	class MathModule : public Module {
	public:
		std::string name = "Math";

		MathModule(Evaluator& evaluator) : Module{evaluator} {
			variables.insert({
				"sin",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arg = valueCast<NumberValue>(args[0])->value;
						return std::make_shared<NumberValue>(sinf(arg));
					}
				)
			});

			variables.insert({
				"cos",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arg = valueCast<NumberValue>(args[0])->value;
						return std::make_shared<NumberValue>(cosf(arg));
					}
				)
			});

			variables.insert({
				"sqrt",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arg = valueCast<NumberValue>(args[0])->value;
						return std::make_shared<NumberValue>(sqrtf(arg));
					}
				)
			});

			variables.insert({
				"pow",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto base = valueCast<NumberValue>(args[0])->value;
						const auto index = valueCast<NumberValue>(args[1])->value;
						return std::make_shared<NumberValue>(powf(base, index));
					}
				)
			});

			variables.insert({
				"PI",
				std::make_shared<NumberValue>(3.141592653589793f)
			});

			variables.insert({
				"E",
				std::make_shared<NumberValue>(2.718281828459045f)
			});

			variables.insert({
				"max",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) -> std::shared_ptr<Value> {
						const auto values = valueCast<ArrayValue>(args[0]);
						if (values->size() == 0)
							return std::make_shared<UndefinedValue>();

						float max = valueCast<NumberValue>(values->at(0))->value;
						for (size_t i = 1; i < values->size(); ++i) {
							float value = valueCast<NumberValue>(values->at(i))->value;
							if (value > max)
								max = value;
						}

						return std::make_shared<NumberValue>(max);
					}
				)
			});

			variables.insert({
				"random",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						std::random_device dev;
						std::mt19937 rng(dev());

						if (args.size() == 0) {
							std::uniform_real_distribution<> dist(0.0, 1.0);
							return std::make_shared<NumberValue>(dist(rng));
						} else if (args.size() == 2) {
							if (args[0]->type != ValueType::NumberValue || args[1]->type != ValueType::NumberValue)
								throw EvaluateError("Math.random accepts either 0 or 2 number arguments");

							float min = valueCast<NumberValue>(args[0])->value;
							float max = valueCast<NumberValue>(args[1])->value;
							std::uniform_real_distribution<> dist(min, max);
							return std::make_shared<NumberValue>(dist(rng));
						}
					}
				)
			});
		}
	};
}