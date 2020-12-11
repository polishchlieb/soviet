#pragma once
#include <string>
#include <unordered_map>
#include "values/FunctionValue.hpp"
#include "ClassObject.hpp"

namespace soviet {
	class Class {
		std::string name;
		std::unordered_map<std::string, FunctionValue> methods;

		ClassObject createInstance();
	};

	ClassObject Class::createInstance() {
		return ClassObject{};
	}
}