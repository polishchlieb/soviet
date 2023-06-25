#pragma once
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
        not_equals_op,
        comma,
        arrow,
        greater_than,
        open_curly_bracket,
        close_curly_bracket,
        dot,
        comment,
        negation,
        less_than_op,
        greater_than_or_equal_op,
        less_than_or_equal_op,
        open_square_bracket,
        close_square_bracket,
        colon,
        or_op,
        and_op,
        pipe_op
    };

    std::string dumpTokenType(const TokenType type);
}
