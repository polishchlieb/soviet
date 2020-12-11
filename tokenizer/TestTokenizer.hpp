#ifndef SOVIET_TESTTOKENIZER_HPP
#define SOVIET_TESTTOKENIZER_HPP

#include "Tokenizer.hpp"

namespace soviet {
    class TestTokenizer : public Tokenizer {
    public:
        Token& peekNextToken() override {
            while (tokens.empty()) {
                if (lines.empty())
                    throw ParseError("expected a token");
                tokenize(lines.front());
                lines.pop();
            }

            return tokens.front();
        }

        Token getNextToken() override {
            while (tokens.empty()) {
                if (lines.empty())
                    throw ParseError("expected a token");
                tokenize(lines.front());
                lines.pop();
            }

            auto value = std::move(tokens.front());
            tokens.pop();

            return value;
        }

        std::vector<Token> getTokens() {
            std::vector<Token> result;
            while (!isComplete())
                result.push_back(getNextToken());
            return std::move(result);
        }

        inline bool isComplete() override {
            return lines.empty() && tokens.empty();
        }

        void addLine(std::string line) {
            lines.push(std::move(line));
        }
    private:
        std::queue<std::string> lines;
    };
}

#endif //SOVIET_TESTTOKENIZER_HPP
