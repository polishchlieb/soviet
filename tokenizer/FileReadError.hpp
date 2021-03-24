#ifndef SOVIET_FILEREADERROR_HPP
#define SOVIET_FILEREADERROR_HPP

#include <exception>
#include "../util/Error.hpp"

namespace soviet {
    class FileReadError {
    public:
        explicit FileReadError(std::string value)
            : value(std::move(value)) {}

        const char* name() const noexcept {
            return "FileReadError";
        }

        std::string message() const noexcept {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_FILEREADERROR_HPP
