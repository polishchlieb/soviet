#pragma once
#include "../util/Error.hpp"

namespace soviet {
    class EvaluateError : public Error {
    public:
        explicit EvaluateError(std::string value);

        std::string name() const noexcept;
        std::string message() const noexcept;
    private:
        std::string value;
    };
}