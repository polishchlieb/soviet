#ifndef SOVIET_ERROR_HPP
#define SOVIET_ERROR_HPP

#include <exception>
#include <string>
#include <iostream>

#include "colors.hpp"

namespace soviet {
    enum class ErrorType {
        no_tokens
    };

    class Error : public std::exception {
    public:
        Error(ErrorType type) : type(type) {}

        ErrorType type;

        const char* name() const {
            return "Error";
        }

        std::string message() const {
            return "";
        }

        void print() const {
            std::cout
                << color(Color::RED)
                << name() << ": " << message()
                << color(Color::RESET) << std::endl;
        }
    };
}

#endif //SOVIET_ERROR_HPP
