#ifndef SOVIET_TOKENITERATOR_HPP
#define SOVIET_TOKENITERATOR_HPP

#include "../tokenizer/Tokens.hpp"

namespace soviet {
    class TokenIterator {
    public:
        TokenIterator() = default;

        explicit TokenIterator(Tokens&& tokens) {
            this->tokens = std::move(tokens);
        }

        Token& getNextToken() {
            return tokens[currentIndex++];
        }

        Token& peekNextToken() {
            return tokens[currentIndex];
        }

        bool isEmpty() {
            return currentIndex - 1 >= tokens.size();
        }
    private:
        Tokens tokens;
        int currentIndex = 0;
    };
}

#endif //SOVIET_TOKENITERATOR_HPP
