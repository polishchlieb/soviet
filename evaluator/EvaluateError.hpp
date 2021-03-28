#ifndef SOVIET_EVALUATEERROR_HPP
#define SOVIET_EVALUATEERROR_HPP

#include <cstring>
#include "../util/Error.hpp"

namespace soviet {
    class EvaluateError : public std::exception {
    public:
        explicit EvaluateError(std::string value)
            : value(std::move(value)) {}

        const char* name() const noexcept {
            return "EvaluateError";
        }

        std::string message() const noexcept {
            return this->value;
        }

        const char* what() const noexcept override {
            std::string result;
            char* _result = _strdup(this->name());
            result += _result;
            result += ": ";
            char* message = _strdup(this->message().c_str());
            result += message;
            return _strdup(result.c_str());
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_EVALUATEERROR_HPP
