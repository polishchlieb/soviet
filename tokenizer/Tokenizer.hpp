#ifndef SOVIET_TOKENIZER_HPP
#define SOVIET_TOKENIZER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Token.hpp"
#include "Tokens.hpp"
#include "../parser/TokenIterator.hpp"

namespace soviet {
    class Tokenizer {
    public:
        void tokenize(const std::string& line) {
            // tokens may have been moved before
            tokens = Tokens();

            // token count is less than or equal the line's length
            tokens.reserve(line.length());

            for (const char c : line) {
                parseChar(c);
            }

            if (!previous.isEmpty()) {
                tokens.add(std::move(previous));
                previous.clear();
            }
        }

        void clear() {
            tokens = Tokens();
        }

        TokenIterator getIterator() {
            return TokenIterator{std::move(tokens)};
        }
    private:
        Tokens tokens;
        Token previous{TokenType::none, ""};

        void parseChar(const char c) {
            switch (previous.type) {
                case TokenType::none: parseNone(c); break;
                case TokenType::string: parseString(c); break;
                case TokenType::number: parseNumber(c); break;
                case TokenType::name: parseName(c); break;
                case TokenType::add_op: parseAddOp(c); break;
                case TokenType::sub_op: parseSubOp(c); break;
                case TokenType::mul_op: parseMulOp(c); break;
                case TokenType::div_op: parseDivOp(c); break;
                case TokenType::open_bracket: parseOpenBracket(c); break;
                case TokenType::close_bracket: parseCloseBracket(c); break;
                case TokenType::equals_op: parseEqualsOp(c); break;
                case TokenType::double_equals_op: parseDoubleEqualsOp(c); break;
                case TokenType::comma: parseComma(c); break;
                default:
                    throw std::runtime_error("not implemented (yet)");
            }
        }

        static TokenType getType(const char c) {
            if (isdigit(c)) return TokenType::number;
            if (isalpha(c)) return TokenType::name;
            if (c == ' ') return TokenType::none;
            if (c == '\'' || c == '"') return TokenType::string;
            if (c == '+') return TokenType::add_op;
            if (c == '-') return TokenType::sub_op;
            if (c == '*') return TokenType::mul_op;
            if (c == '/') return TokenType::div_op;
            if (c == '(') return TokenType::open_bracket;
            if (c == ')') return TokenType::close_bracket;
            if (c == '=') return TokenType::equals_op;
            if (c == ',') return TokenType::comma;
            return TokenType::unknown;
        }

        void parseNone(const char c) {
            const auto type = getType(c);
            switch (getType(c)) {
                case TokenType::number:
                case TokenType::name:
                case TokenType::add_op:
                case TokenType::sub_op:
                case TokenType::div_op:
                case TokenType::mul_op:
                case TokenType::unknown:
                case TokenType::open_bracket:
                case TokenType::close_bracket:
                case TokenType::equals_op:
                case TokenType::comma:
                    previous.type = type;
                    previous.value += c;
                    break;
                case TokenType::string:
                    previous.type = type;
                    break;
                case TokenType::none:
                case TokenType::undefined:
                case TokenType::double_equals_op:
                    break;
            }
        }

        void parseString(const char c) {
            if (c == '\'' || c == '"') {
                tokens.add(std::move(previous));
                previous.clear();
                return;
            }

            previous.value += c;
        }

        void parseNumber(const char c) {
            const auto type = getType(c);
            if (type == TokenType::number) {
                previous.value += c;
                return;
            }

            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseName(const char c) {
            const auto type = getType(c);
            if (type == TokenType::name || type == TokenType::number) {
                previous.value += c;
                return;
            }

            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseAddOp(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseSubOp(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseMulOp(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseDivOp(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseOpenBracket(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseCloseBracket(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseEqualsOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::double_equals_op;
                previous.value += c;
                return;
            }

            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseDoubleEqualsOp(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }

        void parseComma(const char c) {
            tokens.add(std::move(previous));
            previous.clear();
            parseChar(c);
        }
    };
}

#endif //SOVIET_TOKENIZER_HPP
