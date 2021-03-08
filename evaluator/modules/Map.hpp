#pragma once
#include "Module.hpp"
#include "../values/values.hpp"
#include "../EvaluateError.hpp"

namespace soviet {
	class MapModule : public Module {
	public:
        std::string name = "Map";

		MapModule() {
            variables.insert({
                "new",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        if (args.size() == 0)
                            return std::make_shared<MapValue>();

                        auto result = std::make_shared<MapValue>();
                        for (const auto& arg : args) {
                            if (arg->type != ValueType::ArrayValue)
                                throw EvaluateError("map constructor is kaput");
                            const auto entry = valueCast<ArrayValue>(arg);
                            if (entry->size() != 2)
                                throw EvaluateError("map constructor is kaput");

                            result->set(entry->at(0), entry->at(1));
                        }
                        return result;
                    }
                )
            });

            variables.insert({
                "set",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]->clone());
                        map->set(args[1], args[2]);
                        return map;
                    }
                )
            });

            variables.insert({
                "remove",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]->clone());
                        map->remove(args[1]);
                        return map;
                    }
                )
            });

            variables.insert({
                "at",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        return valueCast<MapValue>(args[0])->get(args[1]);
                    }
                )
            });

            variables.insert({
                "entries",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]);
                        const auto result = std::make_shared<ArrayValue>();

                        for (const auto& [key, value] : map->entries())
                            result->add(std::make_shared<ArrayValue>(std::vector{key, value}));

                        return result;
                    }
                )
            });

            variables.insert({
                "keys",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]);
                        return std::make_shared<ArrayValue>(map->keys());
                    }
                )
            });


            variables.insert({
                "values",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]);
                        return std::make_shared<ArrayValue>(map->values());
                    }
                )
            });


            variables.insert({
                "size",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(args[0]);
                        return std::make_shared<NumberValue>(map->size());
                    }
                )
            });
		}
	};
}