#ifndef SOVIET_TOKENTYPE_HPP
#define SOVIET_TOKENTYPE_HPP

#include <string>
#include <stdexcept>
#include "../parser/ParseError.hpp"

namespace soviet {
    enum class TokenType {
        none,
        unknown,
        undefined,
        name,
        number,
        string,
        add_op,
        sub_op,
        div_op,
        mul_op,
        open_bracket,
        close_bracket,
        equals_op,
        double_equals_op,
        comma,
        arrow,
        greater_than,
        open_curly_bracket,
        close_curly_bracket,
        dot,
        comment,
        negation
    };

    std::string dumpTokenType(const TokenType type) {
        switch (type) {
            case TokenType::undefined: return "undefined";
            case TokenType::unknown: return "unknown";
            case TokenType::none: return "none";
            case TokenType::name: return "name";
            case TokenType::number: return "number";
            case TokenType::string: return "string";
            case TokenType::add_op: return "add_operator";
            case TokenType::sub_op: return "substract_operator";
            case TokenType::div_op: return "divide_operator";
            case TokenType::mul_op: return "multiply_operator";
            case TokenType::open_bracket: return "open_bracket";
            case TokenType::close_bracket: return "close_bracket";
            case TokenType::equals_op: return "equals_operator";
            case TokenType::double_equals_op: return "double_equals_operator";
            case TokenType::comma: return "comma";
            case TokenType::arrow: return "arrow";
            case TokenType::greater_than: return "greater_than";
            case TokenType::open_curly_bracket: return "open_curly_bracket";
            case TokenType::close_curly_bracket: return "close_curly_bracket";
            case TokenType::dot: return "dot";
            case TokenType::comment: return "comment";
            case TokenType::negation: return "negation";
            default:
                throw ParseError("Unknown error");
        }
    }
}

#endif //SOVIET_TOKENTYPE_HPP
