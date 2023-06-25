#pragma once
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
        Error(ErrorType type);

        ErrorType type;
        virtual std::string name() const = 0;
        virtual std::string message() const = 0;

        void print() const;
    };
}
