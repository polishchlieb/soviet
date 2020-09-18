#ifndef SOVIET_TOKENTYPE_HPP
#define SOVIET_TOKENTYPE_HPP

#include <string>
#include <stdexcept>

namespace soviet {
    enum class TokenType {
        undefined, // use only when moving Token
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
        comma
    };

    std::string dumpTokenType(const TokenType type) {
        switch (type) {
            case TokenType::undefined:
                throw std::runtime_error("token of type undefined may exists only while moving a token");
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
        }
    }
}

#endif //SOVIET_TOKENTYPE_HPP
