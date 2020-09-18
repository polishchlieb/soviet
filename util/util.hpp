#ifndef SOVIET_UTIL_HPP
#define SOVIET_UTIL_HPP

#include <algorithm>
#include <initializer_list>

namespace soviet {
    template<typename First, typename ...T>
    bool isIn(const First& first, const T&... t) {
        return ((first == t) || ...);
    }
}

#endif //SOVIET_UTIL_HPP
