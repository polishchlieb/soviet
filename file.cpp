#include "file.hpp"
#include "evaluator/evaluator.hpp"

void getfileline(soviet::Evaluator& evaluator, std::ifstream& file, std::string initial) {
	std::string line;
	std::getline(file, line);
	line = initial + line;

	soviet::Tokenizer tokenizer;
	try {
		tokenizer.tokenize(line);
	} catch (const soviet::Error& error) {
		error.print();
		return;
	}

	soviet::Parser parser{tokenizer.getTokens()};

	std::shared_ptr<soviet::Node> tree;
	try {
		tree = parser.parse();
	} catch (const soviet::Error& error) {
		if (error.type == soviet::ErrorType::NoTokens) {
			if (file.eof()) {
				std::cout << "ParseError: parser needs a token, but file finished reading" << std::endl;
				exit(69);
			}

			getfileline(evaluator, file, line);
		} else {
			error.print();
		}
		return;
	}

	try {
		evaluator.evaluate(tree);
	} catch (const soviet::Error& error) {
		error.print();
	}
}
