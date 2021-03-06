#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class ArrayModule : public Module {
	public:
		ArrayModule() {
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
		}
	};
}