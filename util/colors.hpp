#pragma once
#include <string>

namespace soviet {
    enum class Color {
        RED,
        RESET
    };

    const char* color(Color color);
}
