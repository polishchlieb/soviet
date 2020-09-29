#ifndef SOVIET_INLINETOKENIZER_HPP
#define SOVIET_INLINETOKENIZER_HPP

#include "Tokenizer.hpp"

namespace soviet {
    class InlineTokenizer : public Tokenizer {
    public:
        Token& peekNextToken() override {
            while (tokens.empty()) {
                tokenize(getInput());
            }
            return tokens.front();
        }

        Token getNextToken() override {
            while (tokens.empty()) {
                tokenize(getInput());
            }

            auto value = tokens.front();
            tokens.pop();
            return value;
        }

        inline bool isComplete() override {
            return true;
        }
    private:
        std::string getInput() {
            std::cout << ">>> ";
            std::string line;
            std::getline(std::cin, line);

            if (line == "exit")
                exit(0);

            return line;
        }
    };
}

#endif //SOVIET_INLINETOKENIZER_HPP
