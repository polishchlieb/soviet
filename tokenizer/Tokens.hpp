#ifndef SOVIET_TOKENS_HPP
#define SOVIET_TOKENS_HPP

#include <vector>
#include "Token.hpp"

namespace soviet {
    class Tokens : public std::vector<Token> {
    public:
        void add(Token&& token) {
            this->emplace_back(std::move(token));
        }
    };
}

#endif //SOVIET_TOKENS_HPP
