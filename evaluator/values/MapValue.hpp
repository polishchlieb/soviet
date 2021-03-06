#ifndef SOVIET_MAPVALUE_HPP
#define SOVIET_MAPVALUE_HPP

#include "Value.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

namespace soviet {
    class MapValue : public Value {
    private:
        std::unordered_map<std::shared_ptr<Value>, std::shared_ptr<Value>> data;
        typedef std::tuple<std::shared_ptr<Value>, std::shared_ptr<Value>> Entry;
    public:
        explicit MapValue()
            : Value{ValueType::MapValue} {}

        void set(std::shared_ptr<Value> key, std::shared_ptr<Value> value) {
            data.insert({ key, value });
        }

        std::shared_ptr<Value> get(const std::shared_ptr<Value>& key) {
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

        void _delete(const std::shared_ptr<Value>& key) {
            const auto iterator = std::find_if(
                std::begin(data), std::end(data),
                [&key](const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>& entry) {
                    return entry.first->equals(key);
                }
            );
            if (iterator != data.end())
                data.erase(iterator);
        }

        void clear() {
            data.clear();
        }

        size_t size() const {
            return data.size();
        }

        auto keys() const {
            std::vector<std::shared_ptr<Value>> result;
            for (const auto& [key, value] : data)
                result.push_back(key);
            return result;
        }

        auto values() const {
            std::vector<std::shared_ptr<Value>> result;
            for (const auto& [key, value] : data)
                result.push_back(value);
            return result;
        }

        std::vector<Entry> entries() const {
            std::vector<Entry> result;
            for (const auto& [key, value] : data)
                result.push_back(std::make_tuple(key, value));
            return result;
        }

        bool equals(const std::shared_ptr<Value>& other) override {
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

        std::shared_ptr<Value> clone() override {
            auto result = std::make_shared<MapValue>();

            for (const auto& [key, value] : data)
                result->set(key, value);

            return result;
        }
    };
}

#endif