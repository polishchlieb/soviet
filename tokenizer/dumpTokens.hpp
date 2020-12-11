#ifndef SOVIET_DUMPTOKENS_HPP
#define SOVIET_DUMPTOKENS_HPP

#include <vector>
#include <iostream>
#include "Token.hpp"

namespace soviet {
    static void dump(const std::vector<Token>& tokens) {
        for (const auto& token : tokens) {
            std::cout << "(" << dumpTokenType(token.type) << ") " << token.value << std::endl;
        }
    }
}

#endif //SOVIET_DUMPTOKENS_HPP
