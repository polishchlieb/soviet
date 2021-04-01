#pragma once
#include <queue>
#include "NoTokensError.hpp"

namespace soviet {
	class TokenList {
	public:
        explicit TokenList(std::queue<Token> tokens)
            : tokens(std::move(tokens)) {}
        
        void dump() {
            auto copy = tokens;
            while (!copy.empty()) {
                const auto token = copy.front();
                copy.pop();
                std::cout << "(" << dumpTokenType(token.type) << ") " << token.value << std::endl;
            }
        }

        Token& peekNextToken() {
            if (tokens.empty())
                throw NoTokensError{};
            return tokens.front();
        }

        Token getNextToken() {
            if (tokens.empty())
                throw NoTokensError{};
            auto token = std::move(tokens.front());
            tokens.pop();
            return token;
        }

        bool hasNextToken() {
            return !tokens.empty();
        }
    private:
        std::queue<Token> tokens;
	};
}