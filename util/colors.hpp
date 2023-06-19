#ifndef SOVIET_COLORS_HPP
#define SOVIET_COLORS_HPP

#include <string>

namespace soviet {
    enum class Color {
        RED,
        RESET
    };

    static const char* color(Color color) {
        switch (color) {
            case Color::RED:
            #ifdef unix
                return "\x1B[31m";
            #else
                return "";
            #endif
            case Color::RESET:
            #ifdef unix
                return "\033[0m\t\t";
            #else
                return "";
            #endif
            default:
                throw std::runtime_error("Unknown color");
        }
    }
}

#endif //SOVIET_COLORS_HPP
