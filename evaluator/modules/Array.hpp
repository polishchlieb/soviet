#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class ArrayModule : public Module {
	public:
		std::string name = "Array";

		ArrayModule(Evaluator& evaluator) : Module{evaluator} {
			variables.insert({
				"new",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						if (args.size() == 0)
							return std::make_shared<ArrayValue>();

						auto result = std::make_shared<ArrayValue>();
						for (const auto& arg : args)
							result->add(arg);
						return result;
					}
				)
			});

			variables.insert({
				"at",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						auto arr = valueCast<ArrayValue>(args[0]);
						unsigned int index = valueCast<NumberValue>(args[1])->value;
						return arr->at(index);
					}
				)
			});

			variables.insert({
				"remove_at",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						unsigned int index = valueCast<NumberValue>(args[1])->value;
						arr->removeAt(index);
						return arr;
					}
				)
			});

			variables.insert({
				"remove",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						arr->remove(args[1]);
						return arr;
					}
				)
			});

			variables.insert({
				"map",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);

						std::vector<std::shared_ptr<Value>> results;
						for (unsigned int i = 0; i < arr->size(); ++i) {
							auto args = std::vector<std::shared_ptr<Value>>{
								arr->at(i)->clone(),
								std::make_shared<NumberValue>(i)
							};
							results.push_back(callback->run(this->evaluator, args));
						}

						return std::make_shared<ArrayValue>(std::move(results));
					}
				)
			});

			variables.insert({
				"for_each",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);

						for (unsigned int i = 0; i < arr->size(); ++i) {
							auto args = std::vector<std::shared_ptr<Value>>{
								arr->at(i),
								std::make_shared<NumberValue>(i)
							};
							callback->run(this->evaluator, args);
						}

						return std::make_shared<UndefinedValue>();
					}
				)
			});


			variables.insert({
				"add",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						arr->add(args[1]);
						return arr;
					}
				)
			});


			variables.insert({
				"filter",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						const auto callback = valueCast<FunctionValue>(args[1]);

						std::vector<std::shared_ptr<Value>> results;
						for (unsigned int i = 0; i < arr->size(); ++i) {
							const auto& element = arr->at(i);
							auto args = std::vector<std::shared_ptr<Value>>{
								element,
								std::make_shared<NumberValue>(i)
							};
							const auto value = callback->run(this->evaluator, args);
							if (value->type != ValueType::BooleanValue)
								throw EvaluateError("filter callback result must be a boolean");
							const auto filtered = valueCast<BooleanValue>(value)->value;
							if (filtered)
								results.push_back(element);
						}

						return std::make_shared<ArrayValue>(std::move(results));
					}
				)
			});

			variables.insert({
				"contains",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);

						for (const auto& element : arr->getData()) {
							if (element->equals(args[1]))
								return std::make_shared<BooleanValue>(true);
						}

						return std::make_shared<BooleanValue>(false);
					}
				)
			});

			variables.insert({
				"slice",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto start = valueCast<NumberValue>(args[1])->value;
						const auto end = args.size() >= 3
							? valueCast<NumberValue>(args[2])->value
							: arr->size();

						std::vector<std::shared_ptr<Value>> result;
						for (unsigned int i = start; i < end; ++i)
							result.push_back(arr->at(i));

						return std::make_shared<ArrayValue>(std::move(result));
					}
				)
			});

			variables.insert({
				"length",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						return std::make_shared<NumberValue>(arr->size());
					}
				)
			});

			variables.insert({
				"find",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) -> std::shared_ptr<Value> {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);

						for (unsigned int i = 0; i < arr->size(); ++i) {
							const auto& element = arr->at(i);
							auto args = std::vector<std::shared_ptr<Value>>{
								element,
								std::make_shared<NumberValue>(i)
							};

							const auto value = callback->run(this->evaluator, args);
							if (value->type != ValueType::BooleanValue)
								throw EvaluateError("find callback result must be a boolean");

							const auto found = valueCast<BooleanValue>(value)->value;
							if (found)
								return element;
						}

						return std::make_shared<UndefinedValue>();
					}
				)
			});

			variables.insert({
				"find_index",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) -> std::shared_ptr<Value> {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);

						for (unsigned int i = 0; i < arr->size(); ++i) {
							const auto& element = arr->at(i);
							auto args = std::vector<std::shared_ptr<Value>>{
								element,
								std::make_shared<NumberValue>(i)
							};

							const auto value = callback->run(this->evaluator, args);
							if (value->type != ValueType::BooleanValue)
								throw EvaluateError("find callback result must be a boolean");

							const auto found = valueCast<BooleanValue>(value)->value;
							if (found)
								return std::make_shared<NumberValue>(i);
						}

						return std::make_shared<UndefinedValue>();
					}
				)
			});

			variables.insert({
				"index_of",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) -> std::shared_ptr<Value> {
						const auto arr = valueCast<ArrayValue>(args[0]);

						for (unsigned int i = 0; i < arr->size(); ++i) {
							const auto& element = arr->at(i);
							if (element->equals(args[1]))
								return std::make_shared<NumberValue>(i);
						}

						return std::make_shared<UndefinedValue>();
					}
				)
			});

			variables.insert({
				"concat",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arrays = valueCast<ArrayValue>(args[0]);

						const auto result = std::make_shared<ArrayValue>();
						for (const auto& arr : arrays->getData()) {
							if (arr->type == ValueType::ArrayValue) {
								const auto value = valueCast<ArrayValue>(arr);
								for (const auto& element : value->getData())
									result->add(element);
							} else {
								result->add(arr);
							}
						}

						return result;
					}
				)
			});

			variables.insert({
				"some",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);

						for (unsigned int i = 0; i < arr->size(); ++i) {
							const auto& element = arr->at(i);
							std::vector<std::shared_ptr<Value>> args{
								element,
								std::make_shared<NumberValue>(i)
							};
							const auto value = callback->run(this->evaluator, args);
							if (value->type != ValueType::BooleanValue)
								throw EvaluateError("some callback result must be a boolean");
							const auto isTrue = valueCast<BooleanValue>(value)->value;
							if (isTrue)
								return std::make_shared<BooleanValue>(true);
						}

						return std::make_shared<BooleanValue>(false);
					}
				)
			});

			variables.insert({
				"sort",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						const auto compare = valueCast<FunctionValue>(args[1]);
						std::sort(
							arr->getData().begin(), arr->getData().end(),
							[this, &compare](std::shared_ptr<Value> a, std::shared_ptr<Value> b) {
								std::vector<std::shared_ptr<Value>> args{a, b};
								const auto comparison = compare->run(this->evaluator, args);
								return valueCast<BooleanValue>(comparison)->value;
							}
						);
						return arr;
					}
				)
			});

			variables.insert({
				"flat",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						return flat(arr);
					}
				)
			});

			variables.insert({
				"is_array",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						return std::make_shared<BooleanValue>(
							args[0]->type == ValueType::ArrayValue
						);
					}
				)
			});

			variables.insert({
				"reduce",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);
						const auto defaultValue = args[2];

						auto acc = defaultValue;
						for (const auto& element : arr->getData()) {
							std::vector<std::shared_ptr<Value>> args{
								acc,
								element
							};
							acc = callback->run(this->evaluator, args);
						}

						return acc;
					}
				)
			});

			variables.insert({
				"reverse",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]->clone());
						arr->reverse();
						return arr;
					}
				)
			});

			variables.insert({
				"reduce_right",
				std::make_shared<FunctionValue>(
					[this](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto callback = valueCast<FunctionValue>(args[1]);
						const auto defaultValue = args[2];

						auto acc = defaultValue;
						for (auto it = arr->getData().rbegin(); it != arr->getData().rend(); ++it) {
							std::vector<std::shared_ptr<Value>> args{
								acc,
								*it
							};
							acc = callback->run(this->evaluator, args);
						}

						return acc;
					}
				)
			});

			variables.insert({
				"join",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						const auto arr = valueCast<ArrayValue>(args[0]);
						const auto separator = valueCast<StringValue>(args[1])->value;
						
						auto result = std::make_shared<StringValue>("");
						const auto arrSize = arr->size();

						for (unsigned int i = 0; i < arrSize; ++i) {
							result->append(arr->at(i)->dump());
							if (i != arrSize - 1)
								result->append(separator);
						}

						return result;
					}
				)
			});

			variables.insert({
				"range",
				std::make_shared<FunctionValue>(
					[](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
						if (args.size() == 1) {
							if (args[0]->type != ValueType::NumberValue)
								throw EvaluateError("range accepts only number values");
							size_t to = valueCast<NumberValue>(args[0])->value;
							return std::make_shared<RangeValue>(to);
						} else if (args.size() == 2) {
							if (args[0]->type != ValueType::NumberValue || args[1]->type != ValueType::NumberValue)
								throw EvaluateError("range accepts only number values");
							size_t from = valueCast<NumberValue>(args[0])->value;
							size_t to = valueCast<NumberValue>(args[1])->value;
							return std::make_shared<RangeValue>(from, to);
						} else {
							throw EvaluateError("range accepts 1-2 arguments");
						}
					}
				)
			});
		}

	private:
		std::shared_ptr<ArrayValue> flat(std::shared_ptr<ArrayValue> value) {
			auto acc = std::make_shared<ArrayValue>();

			for (const auto& element : value->getData()) {
				if (element->type == ValueType::ArrayValue) {
					const auto arr = valueCast<ArrayValue>(element);
					acc->concat(flat(arr));
				} else {
					acc->add(element);
				}
			}

			return acc;
		}
	};
}