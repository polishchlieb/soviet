#ifndef SOVIET_TOKENIZER_HPP
#define SOVIET_TOKENIZER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Token.hpp"
#include "../parser/ParseError.hpp"
#include <queue>
#include "../util/util.hpp"
#include "TokenList.hpp"

namespace soviet {
    class Tokenizer {
    public:
        void tokenize(const std::string& line);
        TokenList getTokens();

    private:
        std::queue<Token> tokens;
        unsigned int lineNumber = UNDEFINED_LINE;
        Token previous{TokenType::none, "", UNDEFINED_LINE};

        void parseChar(const char c);

        static TokenType getType(const char c);

        void parseNone(const char c);
        void parseString(const char c);
        void parseNumber(const char c);
        void parseName(const char c);
        void parseSubOp(const char c);
        void parseDivOp(const char c);
        void parseEqualsOp(const char c);
        void parseGreaterThan(const char c);
        void parseLessThan(const char c);
        void parseComment(const char c);
        void parseNegation(const char c);
        void parseOr(const char c);
        void parseFixed(const char c);
    };
}

#endif //SOVIET_TOKENIZER_HPP
