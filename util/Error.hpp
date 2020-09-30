#ifndef SOVIET_ERROR_HPP
#define SOVIET_ERROR_HPP

#include <exception>
#include <string>
#include <iostream>

#include "colors.hpp"

namespace soviet {
    class Error : public std::exception {
    public:
        [[nodiscard]] virtual constexpr const char* name() const noexcept = 0;
        [[nodiscard]] virtual std::string message() const noexcept = 0;

        void print() const {
            std::cout
                << color(Color::RED)
                << name() << ": " << message()
                << color(Color::RESET) << std::endl;
        }

        const char* what() const noexcept override {
            return (name() + message()).c_str();
        }
    };
}

#endif //SOVIET_ERROR_HPP
