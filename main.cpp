#include <string>
#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "evaluator/evaluator.hpp"

int main(int argc, char** argv) {
	std::string line = "2 +";
	std::string line2 = "2";
	
	soviet::Tokenizer tokenizer;
	tokenizer.tokenize(line);

	soviet::Parser parser{tokenizer.getTokens()};

	std::shared_ptr<soviet::Node> tree;
	fuk:
	try {
		tree = parser.parse();
	} catch (const soviet::Error& error) {
		if (error.type == soviet::ErrorType::no_tokens) {
			line += line2;
			tokenizer = soviet::Tokenizer{};
			tokenizer.tokenize(line);
			parser = soviet::Parser{tokenizer.getTokens()};
			goto fuk;
		}
	}

	soviet::Evaluator evaluator;
	const auto value = evaluator.evaluate(tree);

	std::cout << soviet::dumpValue(value) << std::endl;
	std::cin.get();
}
