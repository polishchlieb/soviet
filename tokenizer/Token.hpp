#ifndef SOVIET_TOKEN_HPP
#define SOVIET_TOKEN_HPP

#include "TokenType.hpp"
#include <string>

namespace soviet {
    struct Token {
        TokenType type;
        std::string value;
        
        Token(TokenType type, std::string&& value)
            : type(type), value(std::move(value)) {}

        Token(const Token& other)
            : type(other.type), value(other.value) {}

        Token(Token&& other) {
            type = other.type;
            other.type = TokenType::undefined;

            value = std::move(other.value);
        }

        /* explicit Token(PendingToken& token)
            : type(applyPendingTokenType(token.type)),
              value(std::move(token.value)) {} */
    };
}

#endif //SOVIET_TOKEN_HPP
