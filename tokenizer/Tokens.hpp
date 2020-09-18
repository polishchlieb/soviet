#ifndef SOVIET_TOKENS_HPP
#define SOVIET_TOKENS_HPP

#include <vector>
#include "Token.hpp"
#include "PendingToken.hpp"

namespace soviet {
    class Tokens : public std::vector<Token> {
    public:
        void add(PendingToken& token) {
            this->emplace_back(
                applyPendingTokenType(token.type),
                std::move(token.value)
            );
        }
    };
}

#endif //SOVIET_TOKENS_HPP
