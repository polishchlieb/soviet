#include "MapValue.hpp"
#include "../valueCast.hpp"
#include "UndefinedValue.hpp"

namespace soviet {
	MapValue::MapValue()
		: Value{ValueType::MapValue} {}

	void MapValue::set(std::shared_ptr<Value> key, std::shared_ptr<Value> value) {
		data.insert({ key, value });
	}

	std::shared_ptr<Value> MapValue::get(const std::shared_ptr<Value>& key) {
		const auto iterator = std::find_if(
			std::begin(data), std::end(data),
			[&key](const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>& entry) {
				return entry.first->equals(key);
			}
		);
		if (iterator == data.end())
			return std::make_shared<UndefinedValue>();
		return (*iterator).second;
	}

	void MapValue::remove(const std::shared_ptr<Value>& key) {
		const auto iterator = std::find_if(
			std::begin(data), std::end(data),
			[&key](const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>& entry) {
				return entry.first->equals(key);
			}
		);
		if (iterator != data.end())
			data.erase(iterator);
	}

	void MapValue::clear() {
		data.clear();
	}

	size_t MapValue::size() const {
		return data.size();
	}

	std::vector<std::shared_ptr<Value>> MapValue::keys() const {
		std::vector<std::shared_ptr<Value>> result;
		for (const auto& [key, value] : data)
			result.push_back(key);
		return result;
	}

	std::vector<std::shared_ptr<Value>> MapValue::values() const {
		std::vector<std::shared_ptr<Value>> result;
		for (const auto& [key, value] : data)
			result.push_back(value);
		return result;
	}

	std::vector<MapValue::Entry> MapValue::entries() const {
		std::vector<Entry> result;
		for (const auto& [key, value] : data)
			result.push_back(std::make_tuple(key, value));
		return result;
	}

	bool MapValue::equals(const std::shared_ptr<Value>& other) {
		if (other->type != this->type)
			return false;
		const auto otherValue = valueCast<MapValue>(other);
		if (this->size() != otherValue->size())
			return false;
		for (const auto& [key, value] : otherValue->data) {
			if (!value->equals(otherValue->get(key)))
				return false;
		}
		return true;
	}

	std::shared_ptr<Value> MapValue::clone() {
		auto result = std::make_shared<MapValue>();

		for (const auto& [key, value] : data)
			result->set(key, value);

		return result;
	}

	std::string MapValue::dump() const {
		std::string result = "map(";
		for (const auto& [key, value] : entries())
			result += key->dump() + " => " + value->dump() + ", ";
		if (size() == 0)
			return result + ")";
		else
			return result.substr(0, result.length() - 2) + ")";
	}
}