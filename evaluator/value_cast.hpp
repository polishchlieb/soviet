#ifndef SOVIET_VALUE_CAST_HPP
#define SOVIET_VALUE_CAST_HPP

#include <memory>
#include "values/Value.hpp"

namespace soviet {
    template<typename T, typename std::enable_if<std::is_base_of<Value, T>::value>::type* = nullptr>
    inline std::shared_ptr<T> value_cast(const std::shared_ptr<Value>& node) {
        return std::static_pointer_cast<T>(node);
    }
}

#endif //SOVIET_VALUE_CAST_HPP
