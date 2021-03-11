#pragma once
#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class StringModule : public Module {
	public:
		std::string name = "String";

		StringModule() {
			variables.insert({
				"split",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0])->value;
						const auto delimiter = valueCast<StringValue>(args[1])->value;

						auto result = std::make_shared<ArrayValue>();
						auto start = 0U;
						auto end = str.find(delimiter);
						while (end != std::string::npos) {
							std::string element = str.substr(start, end - start);
							result->add(std::make_shared<StringValue>(element));
							start = end + delimiter.length();
							end = str.find(delimiter, start);
						}
						std::string element = str.substr(start, end);
						result->add(std::make_shared<StringValue>(element));

						return result;
					}
				)
			});

			variables.insert({
				"at",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str
							= valueCast<StringValue>(args[0])->value;
						const auto index = static_cast<size_t>(
							valueCast<NumberValue>(args[1])->value
						);

						const auto result = std::string(1, str[index]);
						return std::make_shared<StringValue>(result);
					}
				)
			});

			variables.insert({
				"slice",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str
							= valueCast<StringValue>(args[0])->value;
						const auto start
							= valueCast<NumberValue>(args[1])->value;
						const auto end = args.size() >= 3
							? static_cast<size_t>(valueCast<NumberValue>(args[2])->value)
							: str.length();

						return std::make_shared<StringValue>(str.substr(start, end));
					}
				)
			});

			variables.insert({
				"length",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0]);
						return std::make_shared<NumberValue>(str->length());
					}
				)
			});

			variables.insert({
				"starts_with",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0])->value;
						const auto substr = valueCast<StringValue>(args[1])->value;

						for (unsigned int i = 0; i < substr.length(); ++i) {
							if (str[i] != substr[i])
								return std::make_shared<BooleanValue>(false);
						}

						return std::make_shared<BooleanValue>(true);
					}
				)
			});

			variables.insert({
				"ends_with",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0])->value;
						const auto substr = valueCast<StringValue>(args[1])->value;

						for (int i = str.length() - 1, j = substr.length() - 1; j >= 0; --i, --j) {
							if (str[i] != substr[j])
								return std::make_shared<BooleanValue>(false);
						}

						return std::make_shared<BooleanValue>(true);
					}
				)
			});

			variables.insert({
				"contains",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0])->value;
						const auto substr = valueCast<StringValue>(args[1])->value;
						return std::make_shared<BooleanValue>(
							str.find(substr) != std::string::npos
						);
					}
				)
			});

			variables.insert({
				"replace_all",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0]->clone());
						const auto substr = valueCast<StringValue>(args[1])->value;
						const auto replaceWith = valueCast<StringValue>(args[2])->value;

						str->replaceAll(substr, replaceWith);
						return str;
					}
				)
			});

			variables.insert({
				"from_int",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto value = static_cast<int>(
							valueCast<NumberValue>(args[0])->value
						);
						return std::make_shared<StringValue>(
							std::to_string(value)
						);
					}
				)
			});

			variables.insert({
				"index_of",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0])->value;
						const auto substr = valueCast<StringValue>(args[1])->value;
						return std::make_shared<NumberValue>(str.find(substr));
					}
				)
			});

			variables.insert({
				"to_upper",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0]->clone());
						str->toUpperCase();
						return str;
					}
				)
			});

			variables.insert({
				"to_lower",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0]->clone());
						str->toLowerCase();
						return str;
					}
				)
			});

			variables.insert({
				"characters",
				std::make_shared<FunctionValue>(
					[](std::vector<std::shared_ptr<Value>>& args) {
						const auto str = valueCast<StringValue>(args[0]);
						
						auto result = std::make_shared<ArrayValue>();
						for (const char c : str->value) {
							result->add(
								std::make_shared<StringValue>(std::string(1, c))
							);
						}

						return result;
					}
				)
			});
		}
	};
}