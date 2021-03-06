#ifndef SOVIET_GLOBALSCOPE_HPP
#define SOVIET_GLOBALSCOPE_HPP

#include <iostream>
#include "Scope.hpp"
#include "values/FunctionValue.hpp"
#include "dumpValue.hpp"
#include "EvaluateError.hpp"

namespace soviet {
    class GlobalScope : public Scope {
    public:
        GlobalScope() {
            // std::ios_base::sync_with_stdio(false);

            variables.insert({
                "print",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        for (const auto& arg : args)
                            std::cout << dumpValue(arg) << " ";
                        std::cout << "\n";
                        return std::make_shared<UndefinedValue>();
                    }
                )
            });

            variables.insert({
                "input",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        std::string result;
                        std::getline(std::cin, result);
                        return std::make_shared<StringValue>(result);
                    }
                )
            });

            variables.insert({
                "times",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        const auto times = static_cast<unsigned int>(
                            valueCast<NumberValue>(args[0])->value
                        );
                        const auto callback = valueCast<FunctionValue>(args[1]);

                        for (unsigned int i = 0; i < times; ++i) {
                            std::vector<std::shared_ptr<Value>> callbackArgs = {
                                std::make_shared<NumberValue>(i)
                            };
                            callback->run(callbackArgs);
                        }
                        return std::make_shared<UndefinedValue>();
                    }
                )
            });

            variables.insert({
                "array",
                std::make_shared<FunctionValue>(
                    [](std::vector<std::shared_ptr<Value>>& args) {
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

#endif //SOVIET_GLOBALSCOPE_HPP
