#ifndef SOVIET_EVALUATEERROR_HPP
#define SOVIET_EVALUATEERROR_HPP

#include <cstring>
#include "../util/Error.hpp"

namespace soviet {
    class EvaluateError {
    public:
        explicit EvaluateError(std::string value)
            : value(std::move(value)) {}

        const char* name() const noexcept {
            return "EvaluateError";
        }

        std::string message() const noexcept {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_EVALUATEERROR_HPP
