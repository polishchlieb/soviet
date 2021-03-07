#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class MiscModule : public Module {
	public:
        std::string name = "Misc";

		MiscModule() {
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
		}
	};
}