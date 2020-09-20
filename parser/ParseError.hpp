#ifndef SOVIET_PARSEERROR_HPP
#define SOVIET_PARSEERROR_HPP

#include "../util/Error.hpp"

namespace soviet {
    class ParseError : public Error {
    public:
        explicit ParseError(std::string&& value) {
            this->value = value;
        }

        [[nodiscard]] constexpr const char* name() const noexcept override {
            return "ParseError";
        }

        [[nodiscard]] std::string message() const noexcept override {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_PARSEERROR_HPP
