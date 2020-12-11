#ifndef SOVIET_FILEREADERROR_HPP
#define SOVIET_FILEREADERROR_HPP

#include <exception>
#include "../util/Error.hpp"

namespace soviet {
    class FileReadError : public Error {
    public:
        explicit FileReadError(std::string&& value) {
            this->value = value;
        }

        [[nodiscard]] const char* name() const noexcept override {
            return "FileReadError";
        }

        [[nodiscard]] std::string message() const noexcept override {
            return this->value;
        }
    private:
        std::string value;
    };
}

#endif //SOVIET_FILEREADERROR_HPP
