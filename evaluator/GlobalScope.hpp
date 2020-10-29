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
            std::ios_base::sync_with_stdio(false);

            variables.insert({
                "print",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        for (const auto& arg : args)
                            std::cout << dumpValue(arg) << " ";
                        std::cout << "\n";
                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });

            variables.insert({
                "input",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        std::string result;
                        std::cin >> result;
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
                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });

            variables.insert({
                "object",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        if (args.size() % 2 != 0)
                            throw EvaluateError("object(): Args have to group in pairs");

                        std::unordered_map<std::string, std::shared_ptr<Value>> props;
                        for (unsigned int i = 0; i < args.size(); i += 2) {
                            const auto& name = valueCast<StringValue>(args[i])->value;
                            props.insert({ name, args[i + 1] });
                        }

                        return std::make_shared<ObjectValue>(props);
                    }
                )
            });

            auto arrayPrototype = new PrototypeValue;
            arrayPrototype->registerMethod("at", [](
                std::shared_ptr<Value>& obj,
                std::vector<std::shared_ptr<Value>>& args
            ) {
                const auto array = valueCast<ArrayValue>(obj);
                const auto index = static_cast<unsigned int>(
                    valueCast<NumberValue>(args[0])->value
                );

                return array->at(index);
            });

            variables.insert({
                "array",
                std::make_shared<FunctionValue>(
                    [arrayPrototype](std::vector<std::shared_ptr<Value>>& args) {
                        if (args.empty())
                            return std::make_shared<PrototypeObjectValue>(
                                arrayPrototype,
                                std::make_shared<ArrayValue>()
                            );

                        return std::make_shared<PrototypeObjectValue>(
                            arrayPrototype,
                            std::make_shared<ArrayValue>(std::move(args))
                        );
                    }
                )
            });
        }
    };
}

#endif //SOVIET_GLOBALSCOPE_HPP
