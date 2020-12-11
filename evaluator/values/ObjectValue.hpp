#ifndef SOVIET_OBJECTVALUE_HPP
#define SOVIET_OBJECTVALUE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "Value.hpp"
#include "PrototypeValue.hpp"

namespace soviet {
    struct ObjectValue : Value {
    private:
        typedef std::unordered_map<std::string, std::shared_ptr<Value>> Props;
    public:
        explicit ObjectValue(Props properties)
            : Value{ValueType::ObjectValue}, properties(std::move(properties))
        {}

        ObjectValue() : Value{ValueType::ObjectValue} {}

        void set(const std::string& name, const std::shared_ptr<Value>& value) {
            properties.insert({ name, value });
        }

        [[nodiscard]] bool has(const std::string& property) const {
            return properties.contains(property);
        }

        std::shared_ptr<Value>& get(const std::string& name) {
            return properties[name];
        }

        auto& getProperties() {
            return properties;
        }
    private:
        Props properties;
    };

    struct PrototypeObjectValue : Value {
        explicit PrototypeObjectValue(PrototypeValue* prototype)
            : Value{ValueType::PrototypeObjectValue},
            prototype(prototype) {}
        PrototypeObjectValue(PrototypeValue* prototype, std::shared_ptr<Value> object)
            : Value{ValueType::PrototypeObjectValue},
            prototype(prototype), object(std::move(object)) {}

        PrototypeValue* prototype;
        std::shared_ptr<Value> object;

        std::shared_ptr<Value> call(
            const std::string& methodName,
            std::vector<std::shared_ptr<Value>>& args
        ) const {
            return prototype->call(object, methodName, args);
        }
    };
}

#endif //SOVIET_OBJECTVALUE_HPP
