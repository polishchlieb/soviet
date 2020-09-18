#ifndef SOVIET_TOKENIZER_HPP
#define SOVIET_TOKENIZER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Token.hpp"
#include "PendingToken.hpp"
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
                tokens.add(previous);
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
        PendingToken previous{PendingTokenType::none, ""};

        void parseChar(const char c) {
            switch (previous.type) {
                case PendingTokenType::none: parseNone(c); break;
                case PendingTokenType::string: parseString(c); break;
                case PendingTokenType::number: parseNumber(c); break;
                case PendingTokenType::name: parseName(c); break;
                case PendingTokenType::add_op: parseAddOp(c); break;
                case PendingTokenType::sub_op: parseSubOp(c); break;
                case PendingTokenType::mul_op: parseMulOp(c); break;
                case PendingTokenType::div_op: parseDivOp(c); break;
                case PendingTokenType::open_bracket: parseOpenBracket(c); break;
                case PendingTokenType::close_bracket: parseCloseBracket(c); break;
                case PendingTokenType::equals_op: parseEqualsOp(c); break;
                case PendingTokenType::double_equals_op: parseDoubleEqualsOp(c); break;
                case PendingTokenType::comma: parseComma(c); break;
                default:
                    throw std::runtime_error("not implemented (yet)");
            }
        }

        static PendingTokenType getType(const char c) {
            if (isdigit(c)) return PendingTokenType::number;
            if (isalpha(c)) return PendingTokenType::name;
            if (c == ' ') return PendingTokenType::none;
            if (c == '\'' || c == '"') return PendingTokenType::string;
            if (c == '+') return PendingTokenType::add_op;
            if (c == '-') return PendingTokenType::sub_op;
            if (c == '*') return PendingTokenType::mul_op;
            if (c == '/') return PendingTokenType::div_op;
            if (c == '(') return PendingTokenType::open_bracket;
            if (c == ')') return PendingTokenType::close_bracket;
            if (c == '=') return PendingTokenType::equals_op;
            if (c == ',') return PendingTokenType::comma;
            return PendingTokenType::unknown;
        }

        void parseNone(const char c) {
            const auto type = getType(c);
            switch (getType(c)) {
                case PendingTokenType::number:
                case PendingTokenType::name:
                case PendingTokenType::add_op:
                case PendingTokenType::sub_op:
                case PendingTokenType::div_op:
                case PendingTokenType::mul_op:
                case PendingTokenType::unknown:
                case PendingTokenType::open_bracket:
                case PendingTokenType::close_bracket:
                case PendingTokenType::equals_op:
                case PendingTokenType::comma:
                    previous.type = type;
                    previous.value += c;
                    break;
                case PendingTokenType::string:
                    previous.type = type;
                    break;
                case PendingTokenType::none:
                    break;
            }
        }

        void parseString(const char c) {
            if (c == '\'' || c == '"') {
                tokens.add(previous);
                previous.clear();
                return;
            }

            previous.value += c;
        }

        void parseNumber(const char c) {
            const auto type = getType(c);
            if (type == PendingTokenType::number) {
                previous.value += c;
                return;
            }
            
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseName(const char c) {
            const auto type = getType(c);
            if (type == PendingTokenType::name || type == PendingTokenType::number) {
                previous.value += c;
                return;
            }

            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseAddOp(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseSubOp(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseMulOp(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseDivOp(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseOpenBracket(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseCloseBracket(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseEqualsOp(const char c) {
            const auto type = getType(c);
            if (type == PendingTokenType::equals_op) {
                previous.type = PendingTokenType::double_equals_op;
                previous.value += c;
                return;
            }

            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseDoubleEqualsOp(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }

        void parseComma(const char c) {
            tokens.add(previous);
            previous.clear();
            parseChar(c);
        }
    };
}

#endif //SOVIET_TOKENIZER_HPP
