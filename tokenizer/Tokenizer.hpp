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

namespace soviet {
    class Tokenizer {
    public:
        bool isEmpty() {
            return this->tokens.empty() && this->isComplete();
        }

        virtual bool isComplete() = 0;
        virtual Token& peekNextToken() = 0;
        virtual Token getNextToken() = 0;

        void dump() {
            auto copy = tokens;
            while (!copy.empty()) {
                const auto token = copy.front();
                copy.pop();
                std::cout << "(" << dumpTokenType(token.type) << ") " << token.value << std::endl;
            }
        }

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
    protected:
        std::queue<Token> tokens;
        unsigned int lineNumber = UNDEFINED_LINE;
    private:
        Token previous{TokenType::none, "", UNDEFINED_LINE};

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
                case TokenType::arrow: parseArrow(c); break;
                case TokenType::greater_than: parseGreaterThan(c); break;
                case TokenType::open_curly_bracket: parseOpenCurlyBracket(c); break;
                case TokenType::close_curly_bracket: parseCloseCurlyBracket(c); break;
                case TokenType::dot: parseDot(c); break;
                case TokenType::comment: parseComment(c); break;
                case TokenType::negation: parseNegation(c); break;
                case TokenType::less_than_op: parseLessThan(c); break;
                case TokenType::greater_than_or_equal_op: parseGreaterThanOrEqual(c); break;
                case TokenType::less_than_or_equal_op: parseLessThanOrEqual(c); break;
                case TokenType::open_square_bracket: parseOpenSquareBracket(c); break;
                case TokenType::close_square_bracket: parseCloseSquareBracket(c); break;
                default:
                    throw ParseError("unexpected token on line " + std::to_string(lineNumber));
            }
        }

        static TokenType getType(const char c) {
            if (isdigit(c)) return TokenType::number;
            if (isalpha(c) || c == '_') return TokenType::name;
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
            if (c == '>') return TokenType::greater_than;
            if (c == '<') return TokenType::less_than_op;
            if (c == '{') return TokenType::open_curly_bracket;
            if (c == '}') return TokenType::close_curly_bracket;
            if (c == '.') return TokenType::dot;
            if (c == '!') return TokenType::negation;
            if (c == '[') return TokenType::open_square_bracket;
            if (c == ']') return TokenType::close_square_bracket;
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
                case TokenType::greater_than:
                case TokenType::less_than_op:
                case TokenType::greater_than_or_equal_op:
                case TokenType::less_than_or_equal_op:
                case TokenType::open_curly_bracket:
                case TokenType::close_curly_bracket:
                case TokenType::dot:
                case TokenType::comment:
                case TokenType::negation:
                case TokenType::open_square_bracket:
                case TokenType::close_square_bracket:
                    previous.type = type;
                    previous.value += c;
                    break;
                case TokenType::string:
                    previous.type = type;
                    break;
                case TokenType::none:
                case TokenType::undefined:
                case TokenType::double_equals_op:
                case TokenType::arrow:
                    break;
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
            const auto type = getType(c);
            if (isIn(type, TokenType::number, TokenType::dot)) {
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseName(const char c) {
            const auto type = getType(c);
            if (type == TokenType::name || type == TokenType::number) {
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseAddOp(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseSubOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::greater_than) {
                previous.type = TokenType::arrow;
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseMulOp(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseDivOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::div_op) {
                previous.type = TokenType::comment;
                return;
            }
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseOpenBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseCloseBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseEqualsOp(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::double_equals_op;
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseDoubleEqualsOp(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseComma(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseArrow(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseGreaterThan(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::greater_than_or_equal_op;
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseLessThan(const char c) {
            const auto type = getType(c);
            if (type == TokenType::equals_op) {
                previous.type = TokenType::less_than_or_equal_op;
                previous.value += c;
                return;
            }

            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseGreaterThanOrEqual(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseLessThanOrEqual(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseOpenCurlyBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseCloseCurlyBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseDot(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseComment(const char c) {}

        void parseNegation(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseOpenSquareBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }

        void parseCloseSquareBracket(const char c) {
            tokens.emplace(std::move(previous));
            previous.clear(lineNumber);
            parseChar(c);
        }
    };
}

#endif //SOVIET_TOKENIZER_HPP
