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
            variables.insert({
                "print",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        for (const auto& arg : args)
                            std::cout << dumpValue(arg) << " ";

                        std::cout << std::endl;
                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });
        }
    };
}

#endif //SOVIET_GLOBALSCOPE_HPP
