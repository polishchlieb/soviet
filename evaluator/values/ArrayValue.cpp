#include "ArrayValue.hpp"
#include "../valueCast.hpp"

namespace soviet {
	ArrayValue::ArrayValue(Data data)
		: Value{ValueType::ArrayValue}, data(std::move(data))
	{}

	ArrayValue::ArrayValue() : Value{ValueType::ArrayValue} {}

	void ArrayValue::add(std::shared_ptr<Value> value) {
		data.push_back(std::move(value));
	}

	std::shared_ptr<soviet::Value>& ArrayValue::at(const unsigned int index) {
		return data[index];
	}

	soviet::ArrayValue::Data& ArrayValue::getData() {
		return data;
	}

	size_t ArrayValue::size() const {
		return data.size();
	}

	bool ArrayValue::equals(const std::shared_ptr<Value>& other) {
		if (other->type != this->type)
			return false;
		auto otherValue = valueCast<ArrayValue>(other);
		if (this->size() != otherValue->size())
			return false;
		for (unsigned int i = 0; i < this->size(); ++i) {
			if (!this->at(i)->equals(otherValue->at(i)))
				return false;
		}
		return true;
	}

	std::shared_ptr<soviet::Value> ArrayValue::clone() {
		auto result = std::make_shared<ArrayValue>();
		for (const auto& element : data)
			result->add(element->clone());
		return result;
	}

	void ArrayValue::removeAt(unsigned int index) {
		data.erase(data.begin() + index);
	}

	void ArrayValue::remove(const std::shared_ptr<Value>& element) {
		const auto iterator = std::find_if(
			data.begin(), data.end(),
			[&element](const std::shared_ptr<Value>& curr) {
				return curr->equals(element);
			}
		);
		if (iterator != data.end())
			data.erase(iterator);
	}

	void ArrayValue::concat(const std::shared_ptr<ArrayValue>& other) {
		for (auto element : other->getData()) {
			data.push_back(element);
		}
	}

	void ArrayValue::reverse() {
		std::reverse(data.begin(), data.end());
	}

	std::string ArrayValue::dump() const {
		std::string result = "[";
		const auto max = data.size() - 1;
		for (uint32_t i = 0; i < max + 1; ++i) {
			const auto& element = data[i];
			if (i == max) {
				result += element->dump();
			}
			else {
				result += element->dump() + ", ";
			}
		}
		return result + "]";
	}
}