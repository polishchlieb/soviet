#pragma once
#include <unordered_map>
#include <string>
#include "values/Value.hpp"

namespace soviet {
	class Class;

	class ClassObject {
	public:
		ClassObject() {}
	private:
		class const Class& clazz;
		std::unordered_map<std::string, std::shared_ptr<Value>> properties;
	};
}