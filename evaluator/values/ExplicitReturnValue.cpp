#include "ExplicitReturnValue.hpp"

soviet::ExplicitReturnValue::ExplicitReturnValue(std::shared_ptr<Value> value)
	: Value{ValueType::ExplicitReturnValue}, value(std::move(value))
{}

bool soviet::ExplicitReturnValue::equals(const std::shared_ptr<Value>& other) {
	return false;
}

std::shared_ptr<soviet::Value> soviet::ExplicitReturnValue::clone() {
	return std::make_shared<ExplicitReturnValue>(value->clone());
}

std::string soviet::ExplicitReturnValue::dump() const {
	return "<explicit return value>";
}
