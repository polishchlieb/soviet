#include "Array.hpp"
#include "../EvaluateError.hpp"

namespace soviet {
	ArrayModule::ArrayModule(Evaluator& evaluator) : Module{ evaluator } {
		variables["new"] = std::make_shared<FunctionValue>(&newarray);
		variables["at"] = std::make_shared<FunctionValue>(&at);
		variables["remove_at"] = std::make_shared<FunctionValue>(&remove_at);
		variables["remove"] = std::make_shared<FunctionValue>(&remove);
		variables["remove"] = std::make_shared<FunctionValue>(&remove);
		variables["map"] = std::make_shared<FunctionValue>(&map);
		variables["for_each"] = std::make_shared<FunctionValue>(&for_each);
		variables["add"] = std::make_shared<FunctionValue>(&add);
		variables["filter"] = std::make_shared<FunctionValue>(&filter);
		variables["contains"] = std::make_shared<FunctionValue>(&contains);
		variables["slice"] = std::make_shared<FunctionValue>(&slice);
		variables["length"] = std::make_shared<FunctionValue>(&length);
		variables["find"] = std::make_shared<FunctionValue>(&find);
		variables["find_index"] = std::make_shared<FunctionValue>(&find_index);
		variables["index_of"] = std::make_shared<FunctionValue>(&index_of);
		variables["concat"] = std::make_shared<FunctionValue>(&concat);
		variables["some"] = std::make_shared<FunctionValue>(&some);
		variables["sort"] = std::make_shared<FunctionValue>(&sort);
		variables["flat"] = std::make_shared<FunctionValue>(&flat);
		variables["is_array"] = std::make_shared<FunctionValue>(&is_array);
		variables["reduce"] = std::make_shared<FunctionValue>(&reduce);
		variables["reverse"] = std::make_shared<FunctionValue>(&reverse);
		variables["reduce_right"] = std::make_shared<FunctionValue>(&reduce_right);
		variables["join"] = std::make_shared<FunctionValue>(&join);
		variables["range"] = std::make_shared<FunctionValue>(&range);
	}

	std::shared_ptr<Value> ArrayModule::at(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		auto arr = valueCast<ArrayValue>(args[0]);
		unsigned int index = valueCast<NumberValue>(args[1])->value;
		return arr->at(index);
	}

	std::shared_ptr<Value> ArrayModule::newarray(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		if (args.size() == 0)
			return std::make_shared<ArrayValue>();

		auto result = std::make_shared<ArrayValue>();
		for (const auto& arg : args)
			result->add(arg);
		return result;
	}

	std::shared_ptr<Value> ArrayModule::remove_at(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		unsigned int index = valueCast<NumberValue>(args[1])->value;
		arr->removeAt(index);
		return arr;
	}

	std::shared_ptr<Value> ArrayModule::remove(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		arr->remove(args[1]);
		return arr;
	}

	std::shared_ptr<Value> ArrayModule::map(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);

		std::vector<std::shared_ptr<Value>> results;
		for (unsigned int i = 0; i < arr->size(); ++i) {
			auto args = std::vector<std::shared_ptr<Value>>{
				arr->at(i)->clone(),
				std::make_shared<NumberValue>(i)
			};
			results.push_back(callback->run(evaluator, args));
		}

		return std::make_shared<ArrayValue>(std::move(results));
	}

	std::shared_ptr<Value> ArrayModule::for_each(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);

		for (unsigned int i = 0; i < arr->size(); ++i) {
			auto args = std::vector<std::shared_ptr<Value>>{
				arr->at(i),
				std::make_shared<NumberValue>(i)
			};
			callback->run(evaluator, args);
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> ArrayModule::add(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		arr->add(args[1]);
		return arr;
	}

	std::shared_ptr<Value> ArrayModule::filter(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		const auto callback = valueCast<FunctionValue>(args[1]);

		std::vector<std::shared_ptr<Value>> results;
		for (unsigned int i = 0; i < arr->size(); ++i) {
			const auto& element = arr->at(i);
			auto args = std::vector<std::shared_ptr<Value>>{
				element,
				std::make_shared<NumberValue>(i)
			};
			const auto value = callback->run(evaluator, args);
			if (value->type != ValueType::BooleanValue)
				throw EvaluateError("filter callback result must be a boolean");
			const auto filtered = valueCast<BooleanValue>(value)->value;
			if (filtered)
				results.push_back(element);
		}

		return std::make_shared<ArrayValue>(std::move(results));
	}

	std::shared_ptr<Value> ArrayModule::contains(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);

		for (const auto& element : arr->getData()) {
			if (element->equals(args[1]))
				return std::make_shared<BooleanValue>(true);
		}

		return std::make_shared<BooleanValue>(false);
	}

	std::shared_ptr<Value> ArrayModule::slice(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
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

	std::shared_ptr<Value> ArrayModule::length(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		return std::make_shared<NumberValue>(arr->size());
	}

	std::shared_ptr<Value> ArrayModule::find(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);

		for (unsigned int i = 0; i < arr->size(); ++i) {
			const auto& element = arr->at(i);
			auto args = std::vector<std::shared_ptr<Value>>{
				element,
				std::make_shared<NumberValue>(i)
			};

			const auto value = callback->run(evaluator, args);
			if (value->type != ValueType::BooleanValue)
				throw EvaluateError("find callback result must be a boolean");

			const auto found = valueCast<BooleanValue>(value)->value;
			if (found)
				return element;
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> ArrayModule::find_index(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);

		for (unsigned int i = 0; i < arr->size(); ++i) {
			const auto& element = arr->at(i);
			auto args = std::vector<std::shared_ptr<Value>>{
				element,
				std::make_shared<NumberValue>(i)
			};

			const auto value = callback->run(evaluator, args);
			if (value->type != ValueType::BooleanValue)
				throw EvaluateError("find callback result must be a boolean");

			const auto found = valueCast<BooleanValue>(value)->value;
			if (found)
				return std::make_shared<NumberValue>(i);
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> ArrayModule::index_of(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);

		for (unsigned int i = 0; i < arr->size(); ++i) {
			const auto& element = arr->at(i);
			if (element->equals(args[1]))
				return std::make_shared<NumberValue>(i);
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> ArrayModule::concat(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
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

	std::shared_ptr<Value> ArrayModule::some(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);

		for (unsigned int i = 0; i < arr->size(); ++i) {
			const auto& element = arr->at(i);
			std::vector<std::shared_ptr<Value>> args{
				element,
					std::make_shared<NumberValue>(i)
			};
			const auto value = callback->run(evaluator, args);
			if (value->type != ValueType::BooleanValue)
				throw EvaluateError("some callback result must be a boolean");
			const auto isTrue = valueCast<BooleanValue>(value)->value;
			if (isTrue)
				return std::make_shared<BooleanValue>(true);
		}

		return std::make_shared<BooleanValue>(false);
	}

	std::shared_ptr<Value> ArrayModule::sort(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		const auto compare = valueCast<FunctionValue>(args[1]);
		std::sort(
			arr->getData().begin(), arr->getData().end(),
			[&evaluator, &compare](std::shared_ptr<Value> a, std::shared_ptr<Value> b) {
				std::vector<std::shared_ptr<Value>> args{a, b};
				const auto comparison = compare->run(evaluator, args);
				return valueCast<BooleanValue>(comparison)->value;
			}
		);
		return arr;
	}

	std::shared_ptr<Value> ArrayModule::flat(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		return _flat(arr);
	}

	std::shared_ptr<Value> ArrayModule::is_array(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		return std::make_shared<BooleanValue>(
			args[0]->type == ValueType::ArrayValue
		);
	}

	std::shared_ptr<Value> ArrayModule::reduce(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);
		const auto defaultValue = args[2];

		auto acc = defaultValue;
		for (const auto& element : arr->getData()) {
			std::vector<std::shared_ptr<Value>> args{acc, element};
			acc = callback->run(evaluator, args);
		}

		return acc;
	}

	std::shared_ptr<Value> ArrayModule::reverse(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]->clone());
		arr->reverse();
		return arr;
	}

	std::shared_ptr<Value> ArrayModule::reduce_right(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto callback = valueCast<FunctionValue>(args[1]);
		const auto defaultValue = args[2];

		auto acc = defaultValue;
		for (auto it = arr->getData().rbegin(); it != arr->getData().rend(); ++it) {
			std::vector<std::shared_ptr<Value>> args{acc, *it};
			acc = callback->run(evaluator, args);
		}

		return acc;
	}

	std::shared_ptr<Value> ArrayModule::join(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto arr = valueCast<ArrayValue>(args[0]);
		const auto& separator = valueCast<StringValue>(args[1])->value;

		auto result = std::make_shared<StringValue>("");
		const auto arrSize = arr->size();

		for (unsigned int i = 0; i < arrSize; ++i) {
			result->append(arr->at(i)->dump());
			if (i != arrSize - 1)
				result->append(separator);
		}

		return result;
	}

	std::shared_ptr<Value> ArrayModule::range(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
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

	std::shared_ptr<ArrayValue> ArrayModule::_flat(std::shared_ptr<ArrayValue> value) {
		auto acc = std::make_shared<ArrayValue>();

		for (const auto& element : value->getData()) {
			if (element->type == ValueType::ArrayValue) {
				const auto arr = valueCast<ArrayValue>(element);
				acc->concat(_flat(arr));
			} else {
				acc->add(element);
			}
		}

		return acc;
	}
}