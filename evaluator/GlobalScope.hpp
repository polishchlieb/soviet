#ifndef SOVIET_GLOBALSCOPE_HPP
#define SOVIET_GLOBALSCOPE_HPP

#include <iostream>
#include "Scope.hpp"
#include "values/FunctionValue.hpp"
#include "dumpValue.hpp"

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
                            std::cout << dumpValue(arg);
                        std::cout << "\n";
                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });

            variables.insert({
                "times",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        const auto times = static_cast<int>(
                            value_cast<NumberValue>(args[0])->value
                        );
                        const auto callback = value_cast<FunctionValue>(args[1]);

                        for (unsigned int i = 0; i < times; ++i)
                            callback->run({
                                std::make_shared<NumberValue>(i)
                            });
                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });
        }
    };
}

#endif //SOVIET_GLOBALSCOPE_HPP
