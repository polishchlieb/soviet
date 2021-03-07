#ifndef SOVIET_SCOPE_HPP
#define SOVIET_SCOPE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "values/Value.hpp"
#include "modules/Module.hpp"

namespace soviet {
    class Scope {
    public:
        void merge(Scope& other) {
            this->merge(&other);
        }

        void merge(Scope* other) {
            for (const auto& [name, value] : other->variables)
                variables.insert({ name, value });
        }

        std::unordered_map<std::string, std::shared_ptr<Value>> variables;
        std::unordered_map<std::string, Module*> modules;
    };
}

#endif //SOVIET_SCOPE_HPP
