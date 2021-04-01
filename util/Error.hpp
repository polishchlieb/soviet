#ifndef SOVIET_ERROR_HPP
#define SOVIET_ERROR_HPP

#include <exception>
#include <string>
#include <iostream>

#include "colors.hpp"

namespace soviet {
    enum class ErrorType {
        NoTokens,
        Evaluate,
        Parse
    };
    
    class Error : public std::exception {
    public:
        Error(ErrorType type) : type(type) {}

        ErrorType type;
        virtual std::string name() const = 0;
        virtual std::string message() const = 0;

        void print() const {
            std::cerr
                << color(Color::RED)
                << name() << ": " << message()
                << color(Color::RESET) << std::endl;
        }
    };
}

#endif //SOVIET_ERROR_HPP
