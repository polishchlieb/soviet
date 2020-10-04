#ifndef SOVIET_TOKEN_HPP
#define SOVIET_TOKEN_HPP

#include "TokenType.hpp"
#include <string>
#include <utility>

namespace soviet {
    struct Token {
        TokenType type;
        std::string value;
        
        Token(TokenType type, std::string&& value)
            : type(type), value(std::move(value)) {}

        Token(const Token& other) = default;

        Token(Token&& other) noexcept
          : value(std::move(other.value)),
          type(std::exchange(other.type, TokenType::undefined)) {}

        Token& operator=(const Token& t) = default;

        [[nodiscard]] bool isEmpty() const {
            return this->type == TokenType::none;
        }

        void clear() {
            this->type = TokenType::none;
            this->value = "";
        }
    };
}

#endif //SOVIET_TOKEN_HPP
