#pragma once
#include "Module.hpp"
#include "../values/values.hpp"
#include "../EvaluateError.hpp"

namespace soviet {
	class MapModule : public Module {
	public:
		MapModule() {
            variables.insert({
                "map",
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
                "map_set",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        const auto map = valueCast<MapValue>(
                            valueCast<MapValue>(args[0])->clone()
                        );
                        map->set(args[1], args[2]);
                        return map;
                    }
                )
            });

            variables.insert({
                "map_at",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
                        return valueCast<MapValue>(args[0])->get(args[1]);
                    }
                )
            });
		}
	};
}