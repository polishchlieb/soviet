#ifndef SOVIET_FILETOKENIZER_HPP
#define SOVIET_FILETOKENIZER_HPP

#include <fstream>
#include "Tokenizer.hpp"

namespace soviet {
    class FileTokenizer : public Tokenizer {
    public:
        explicit FileTokenizer(const char* fileName) {
            std::ifstream file(fileName);
            std::string line;
            while (std::getline(file, line))
                lines.emplace(std::move(line));
        }

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

            auto value = tokens.front();
            tokens.pop();
            return value;
        }

        inline bool isComplete() override {
            return lines.empty();
        }
    private:
        std::queue<std::string> lines;
    };
}

#endif //SOVIET_FILETOKENIZER_HPP
