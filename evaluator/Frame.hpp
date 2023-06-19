#pragma once
#include <unordered_map>
#include "values/Value.hpp"
#include <memory>

namespace soviet {
	class Frame {
		std::unordered_map<std::string, std::shared_ptr<Value>> names;
		std::string id;

		std::shared_ptr<Value> resolveName(const std::string& name) {
		
		}
	};
}