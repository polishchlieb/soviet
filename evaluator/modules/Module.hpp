#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../values/Value.hpp"

namespace soviet {
	class Module {
	public:
		std::string name;
		std::unordered_map<std::string, std::shared_ptr<Value>> variables;
	};
}