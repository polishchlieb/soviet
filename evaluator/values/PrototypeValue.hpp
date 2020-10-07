#ifndef SOVIET_PROTOTYPEVALUE_HPP
#define SOVIET_PROTOTYPEVALUE_HPP

#include <unordered_map>
#include <string>
#include "Value.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace soviet {
    class PrototypeValue : Value {
    private:
        typedef std::function<std::shared_ptr<Value>(
            std::shared_ptr<Value>&,
            std::vector<std::shared_ptr<Value>>&
        )> Callback;
    public:
        PrototypeValue() : Value{ValueType::PrototypeValue} {}

        void registerMethod(std::string name, Callback callback) {
            methods.insert({ name, callback });
        }

        std::shared_ptr<Value> call(
            std::shared_ptr<Value> obj,
            const std::string& methodName,
            std::vector<std::shared_ptr<Value>>& args
        ) {
            return methods[methodName](obj, args);
        }

        std::string testName = "prototype";
    private:
        std::unordered_map<std::string, Callback> methods;
    };
}

#endif //SOVIET_PROTOTYPEVALUE_HPP
