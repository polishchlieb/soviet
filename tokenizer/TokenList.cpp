#include "TokenList.hpp"

namespace soviet {
	TokenList::TokenList(std::queue<Token> tokens)
		: tokens(std::move(tokens))
	{}

	void TokenList::dump() {
		std::queue<Token> copy = tokens;
		while (!copy.empty()) {
			Token token = copy.front();
			copy.pop();
			std::cout << "(" << dumpTokenType(token.type) << ") " << token.value << std::endl;
		}
	}

	Token& TokenList::peekNextToken() {
		if (tokens.empty())
			throw NoTokensError{};
		return tokens.front();
	}

	Token TokenList::getNextToken() {
		if (tokens.empty())
			throw NoTokensError{};
		Token token = std::move(tokens.front());
		tokens.pop();
		return token;
	}

	bool TokenList::hasNextToken() {
		return !tokens.empty();
	}
}