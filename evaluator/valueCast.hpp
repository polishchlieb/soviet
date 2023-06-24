#pragma once
#include <memory>
#include "values/Value.hpp"

namespace soviet {
    template<typename T, typename std::enable_if<std::is_base_of<Value, T>::value>::type* = nullptr>
    inline std::shared_ptr<T> valueCast(const std::shared_ptr<Value>& node) {
        return std::static_pointer_cast<T>(node);
    }
}
