#ifndef SOVIET_EVALUATEERROR_HPP
#define SOVIET_EVALUATEERROR_HPP

#include <cstring>
#include "../util/Error.hpp"

namespace soviet {
    class EvaluateError : public Error {
    public:
        explicit EvaluateError(std::string value)
            : Error{ErrorType::Evaluate}, value(std::move(value)) {}

        std::string name() const noexcept {
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
