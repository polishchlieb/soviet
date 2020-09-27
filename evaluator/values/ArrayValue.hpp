#ifndef SOVIET_ARRAYVALUE_HPP
#define SOVIET_ARRAYVALUE_HPP

#include <vector>
#include "Value.hpp"
#include <memory>

namespace soviet {
    struct ArrayValue : ObjectValue {
    public:
        ArrayValue() : ObjectValue{} {
            registerFunctionality();
        }

        explicit ArrayValue(std::vector<std::shared_ptr<Value>>&& data)
            : ObjectValue{}, data(std::move(data))
        {
            registerFunctionality();
        }
    private:
        std::vector<std::shared_ptr<Value>> data;

        // TODO: Create a system of prototypes
        // This code is extremely inefficient when creating
        // a large amount of objects, these lambdas get
        // allocated each time ArrayValue gets constructed
        void registerFunctionality() {
            this->set("at", std::make_shared<FunctionValue>(
                [this](const std::vector<std::shared_ptr<Value>>& args) {
                    const auto index = static_cast<unsigned int>(
                        value_cast<NumberValue>(args[0])->value
                    );
                    return data[index];
                }
            ));

            this->set("add", std::make_shared<FunctionValue>(
                [this](const std::vector<std::shared_ptr<Value>>& args) {
                    for (const auto& arg : args)
                        data.push_back(arg);
                    return std::make_shared<Value>(ValueType::UndefinedValue);
                }
            ));

            this->set("each", std::make_shared<FunctionValue>(
                [this](const std::vector<std::shared_ptr<Value>>& args) {
                    const auto callback = value_cast<FunctionValue>(args[0]);
                    for (const auto& element : data) {
                        std::vector<std::shared_ptr<Value>> callbackArgs
                            = { element };
                        callback->run(callbackArgs);
                    }
                    return std::make_shared<Value>(ValueType::UndefinedValue);
                }
            ));
        }
    };
}

#endif //SOVIET_ARRAYVALUE_HPP