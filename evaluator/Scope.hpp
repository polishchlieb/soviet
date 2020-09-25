#ifndef SOVIET_SCOPE_HPP
#define SOVIET_SCOPE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "values/Value.hpp"

namespace soviet {
    class Scope {
    public:
        std::unordered_map<std::string, std::shared_ptr<Value>> variables;
    };
}

#endif //SOVIET_SCOPE_HPP
