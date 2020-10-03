#ifndef SOVIET_OBJECTVALUE_HPP
#define SOVIET_OBJECTVALUE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "Value.hpp"

namespace soviet {
    struct ObjectValue : Value {
    private:
        typedef std::unordered_map<std::string, std::shared_ptr<Value>> Props;
        Props properties;

    public:
        explicit ObjectValue(Props properties)
            : Value{ValueType::ObjectValue}, properties(std::move(properties))
        {}

        ObjectValue() : Value{ValueType::ObjectValue} {}

        void set(const std::string& name, const std::shared_ptr<Value>& value) {
            properties.insert({ name, value });
        }

        std::shared_ptr<Value>& get(const std::string& name) {
            return properties[name];
        }

        auto& getProperties() {
            return properties;
        }
    };
}

#endif //SOVIET_OBJECTVALUE_HPP
