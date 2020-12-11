#ifndef SOVIET_EVALUATEERROR_HPP
#define SOVIET_EVALUATEERROR_HPP

#include <cstring>
#include "../util/Error.hpp"

namespace soviet {
    class EvaluateError : public Error {
    public:
        explicit EvaluateError(std::string&& value) {
            this->value = value;
        }

        [[nodiscard]] const char* name() const noexcept override {
            return "EvaluateError";
        }

        [[nodiscard]] std::string message() const noexcept override {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_EVALUATEERROR_HPP
