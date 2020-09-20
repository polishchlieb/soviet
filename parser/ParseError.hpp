#ifndef SOVIET_PARSEERROR_HPP
#define SOVIET_PARSEERROR_HPP

#include "../util/Error.hpp"

namespace soviet {
    class ParseError : public Error {
    public:
        explicit ParseError(const char* value) {
            this->value = value;
        }

        [[nodiscard]] constexpr const char* name() const noexcept override {
            return "ParseError";
        }

        [[nodiscard]] const char* what() const noexcept override {
            return this->value;
        }
    private:
        const char* value;
    };
}

#endif //SOVIET_PARSEERROR_HPP
