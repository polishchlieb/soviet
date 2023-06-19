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
        void tokenize(const std::string& line) {
            previous.clear(++lineNumber);

            for (const char c : line) {
                parseChar(c);
            }

            if (!previous.isEmpty()) {
                if (previous.type == TokenType::string)
                    throw ParseError(
                        "expected string ending on line " + std::to_string(previous.line)
                    );
                if (previous.type == TokenType::comment)
                    return;

                tokens.emplace(std::move(previous));
            }
        }

        TokenList getTokens() {
            return TokenList{tokens};
        }
    private:
        std::queue<Token> tokens;
        unsigned int lineNumber = UNDEFINED_LINE;
        Token previous{TokenType::none, "", UNDEFINED_LINE};

        void parseChar(const char c) {
            switch (previous.type) {
                case TokenType::none: parseNone(c); break;
                case TokenType::string: parseString(c); break;
                case TokenType::number: parseNumber(c); break;
                case TokenType::name: parseName(c); break;
                case TokenType::sub_op: parseSubOp(c); break;
                case TokenType::div_op: parseDivOp(c); break;
                case TokenType::equals_op: parseEqualsOp(c); break;
                case TokenType::greater_than: parseGreaterThan(c); break;
                case TokenType::comment: parseComment(c); break;
                case TokenType::negation: parseNegation(c); break;
                case TokenType::less_than_op: parseLessThan(c); break;
                case TokenType::or_op: parseOr(c); break;
                case TokenType::greater_than_or_equal_op:
                case TokenType::less_than_or_equal_op:
                case TokenType::open_square_bracket:
                case TokenType::close_square_bracket:
                case TokenType::colon:
                case TokenType::dot:
                case TokenType::not_equals_op:
                case TokenType::add_op:
                case TokenType::mul_op:
                case TokenType::open_bracket:
                case TokenType::close_bracket:
                case TokenType::open_curly_bracket:
                case TokenType::close_curly_bracket:
                case TokenType::double_equals_op:
                case TokenType::comma:
                case TokenType::arrow:
                case TokenType::pipe_op:
                case TokenType::and_op:
                    parseFixed(c);
                    break;
                default:
                    throw ParseError("unexpected token on line " + std::to_string(lineNumber));
            }
        }

        static TokenType getType(const char c) {
            if (isdigit(c)) return TokenType::number;
            if (isalpha(c) || c == '_') return TokenType::name;
            switch (c) {
                case ' ': return TokenType::none;
                case '\'': case '"': return TokenType::string;
                case '+': return TokenType::add_op;
                case '-': return TokenType::sub_op;
                case '*': return TokenType::mul_op;
                case '/': return TokenType::div_op;
                case '(': return TokenType::open_bracket;
                case ')': return TokenType::close_bracket;
                case '=': return TokenType::equals_op;
                case ',': return TokenType::comma;
                case '>': return TokenType::greater_than;
                case '<': return TokenType::less_than_op;
                case '{': return TokenType::open_curly_bracket;
                case '}': return TokenType::close_curly_bracket;
                case '.': return TokenType::dot;
                case '!': return TokenType::negation;
                case '[': return TokenType::open_square_bracket;
                case ']': return TokenType::close_square_bracket;
                case ':': return TokenType::colon;
                case '|': return TokenType::or_op;
                case '&': return TokenType::and_op;
            }
            return TokenType::unknown;
        }

        void parseNone(const char c) {
            const auto type = getType(c);

            if (type == TokenType::string) {
                previous.type = type;
            } else if (type != TokenType::none) {
                previous.type = type;
                previous.value += c;
            }
        }

        void parseString(const char c) {
            if (c == '\'' || c == '"') {
                tokens.emplace(std::move(previous));
                previous.clear(lineNumber);
                return;
            }

            previous.value += c;
        }

        void parseNumber(const char c) {
            if (isIn(getType(c), TokenType::number, TokenType::dot)) {
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseName(const char c) {
            if (isIn(getType(c), TokenType::name, TokenType::number)) {
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseSubOp(const char c) {
            if (getType(c) == TokenType::greater_than) {
                previous.type = TokenType::arrow;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseDivOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::div_op) {
                previous.type = TokenType::comment;
                return;
            }

            parseFixed(c);
        }

        void parseEqualsOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::double_equals_op;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseGreaterThan(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::greater_than_or_equal_op;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseLessThan(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::less_than_or_equal_op;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseComment(const char c) {}

        void parseNegation(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::not_equals_op;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseOr(const char c) {
            const auto type = getType(c);
            if (type == TokenType::greater_than) {
                previous.type = TokenType::pipe_op;
                previous.value += c;
                return;
            }

            parseFixed(c);
        }

        void parseFixed(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }
    };
}

#endif //SOVIET_TOKENIZER_HPP
