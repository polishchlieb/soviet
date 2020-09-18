#ifndef SOVIET_FUNCTION_HPP
#define SOVIET_FUNCTION_HPP

#include <functional>
#include <string>
#include <memory>
#include "values/Value.hpp"

namespace soviet {
    struct Function {
        std::string name;
        std::function<
            std::shared_ptr<Value>(
                const std::vector<std::shared_ptr<Value>>&
            )
        > callback;
    };
}

#endif //SOVIET_FUNCTION_HPP
