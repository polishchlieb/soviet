#pragma once
#include "Module.hpp"
#include "../values/values.hpp"
#include "../dumpValue.hpp"
#include <iostream>

namespace soviet {
	class IOModule : public Module {
	public:
		IOModule() {
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
		}
	};
}