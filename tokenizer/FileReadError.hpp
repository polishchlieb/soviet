#pragma once
#include <exception>
#include <string>

namespace soviet {
    class FileReadError {
    public:
        explicit FileReadError(std::string value);

        const char* name() const noexcept;
        std::string message() const noexcept;
    private:
        std::string value;
    };
}
