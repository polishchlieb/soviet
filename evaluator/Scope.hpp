#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "values/Value.hpp"
#include "modules/Module.hpp"

namespace soviet {
    class Scope {
    public:
        Scope(Evaluator& evaluator);
        ~Scope();

        void merge(Scope& other);
        void merge(Scope* other);

        std::unordered_map<std::string, std::shared_ptr<Value>> variables;
        std::unordered_map<std::string, Module*> modules;
    protected:
        Evaluator& evaluator;
    };
}
