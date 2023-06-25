#pragma once
#include "TokenType.hpp"
#include <string>

namespace soviet {
    static constexpr unsigned int UNDEFINED_LINE = 0;

    struct Token {
        TokenType type;
        std::string value;
        unsigned int line;
        
        Token(TokenType type, std::string&& value, unsigned int line);
        Token(const Token& other) = default;
        Token(Token&& other) noexcept;

        Token& operator=(const Token& t) = default;

        bool isEmpty() const;
        void clear(unsigned int line);
    };
}
