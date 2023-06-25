#include "repl.hpp"

static soviet::Thread mainThread;

void getline(std::string initial) {
	std::cout << ">>> ";

	std::string line;
	std::getline(std::cin, line);
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
			getline(line);
		} else {
			error.print();
		}
		return;
	}

	try {
		const auto value = mainThread.evaluator->evaluate(tree);
		std::cout << value->dump() << std::endl;
	} catch (const soviet::Error& error) {
		error.print();
	}
}
