#ifndef SOVIET_TOKEN_HPP
#define SOVIET_TOKEN_HPP

#include "TokenType.hpp"
#include <string>
#include <utility>

namespace soviet {
    static constexpr unsigned int UNDEFINED_LINE = 0;

    struct Token {
        TokenType type;
        std::string value;
        unsigned int line;
        
        Token(TokenType type, std::string&& value, unsigned int line)
            : type(type), value(std::move(value)), line(line) {}

        Token(const Token& other) = default;

        Token(Token&& other) noexcept
          : type(std::exchange(other.type, TokenType::undefined)),
          value(std::move(other.value)),
          line(std::exchange(other.line, UNDEFINED_LINE)) {}

        Token& operator=(const Token& t) = default;

        bool isEmpty() const {
            return this->type == TokenType::none;
        }

        void clear(unsigned int line) {
            this->type = TokenType::none;
            this->value = "";
            this->line = line;
        }
    };
}

#endif //SOVIET_TOKEN_HPP
