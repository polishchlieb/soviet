#ifndef SOVIET_PARSEERROR_HPP
#define SOVIET_PARSEERROR_HPP

#include "../util/Error.hpp"

namespace soviet {
    class ParseError : public Error {
    public:
        explicit ParseError(std::string value)
            : Error{ErrorType::Parse}, value(std::move(value)) {}

        std::string name() const noexcept {
            return "ParseError";
        }

        std::string message() const noexcept {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_PARSEERROR_HPP
