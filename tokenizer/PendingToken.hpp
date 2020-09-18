#ifndef SOVIET_PENDINGTOKEN_HPP
#define SOVIET_PENDINGTOKEN_HPP

#include <string>
#include <vector>
#include "PendingTokenType.hpp"
#include "Token.hpp"

namespace soviet {
    struct PendingToken {
        PendingTokenType type;
        std::string value;

        PendingToken(PendingTokenType type, std::string&& value)
            : type(type), value(std::move(value)) {}       

        PendingToken(const PendingToken& other) = default;

        PendingToken(PendingToken&& other) noexcept
            : type(other.type), value(std::move(other.value)) {
            other.type = PendingTokenType::none;
        }

        void clear() {
            this->type = PendingTokenType::none;
            this->value = "";
        }

        [[nodiscard]] bool isEmpty() const {
            return this->type == PendingTokenType::none;
        }
    };
}

#endif //SOVIET_PENDINGTOKEN_HPP
