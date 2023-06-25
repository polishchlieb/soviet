#pragma once
#include "Value.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace soviet {
    class MapValue : public Value {
    private:
        std::unordered_map<std::shared_ptr<Value>, std::shared_ptr<Value>> data;
        typedef std::tuple<std::shared_ptr<Value>, std::shared_ptr<Value>> Entry;
    public:
        explicit MapValue();

        void set(std::shared_ptr<Value> key, std::shared_ptr<Value> value);
        std::shared_ptr<Value> get(const std::shared_ptr<Value>& key);
        void remove(const std::shared_ptr<Value>& key);
        void clear();
        size_t size() const;
        std::vector<std::shared_ptr<Value>> keys() const;
        std::vector<std::shared_ptr<Value>> values() const;
        std::vector<Entry> entries() const;
        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        std::string dump() const override;
    };
}