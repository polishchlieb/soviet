#include <string>
#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "evaluator/evaluator.hpp"

static void getline(soviet::Evaluator& evaluator, std::string initial = "") {
	// TODO: do sth with tokenizer, parser maybe
	std::cout << ">>> ";

	std::string line;
	std::getline(std::cin, line);
	line = initial + line;

	soviet::Tokenizer tokenizer;
	tokenizer.tokenize(line);

	soviet::Parser parser{ tokenizer.getTokens() };

	std::shared_ptr<soviet::Node> tree;
	try {
		tree = parser.parse();
	}
	catch (const soviet::Error& error) {
		if (error.type == soviet::ErrorType::no_tokens) {
			getline(evaluator, line);
			return;
		}
	}

	try {
		const auto value = evaluator.evaluate(tree);
		std::cout << value->dump() << std::endl;
	}
	catch (const std::exception& error) {
		std::cerr << "[oostszeszenie cos poszlo nie tak] " << error.what() << std::endl;
	}
}

#include <fstream>

static void getfileline(soviet::Evaluator& evaluator, std::ifstream& file, std::string initial = "") {
	std::string line;
	std::getline(file, line);
	line = initial + line;

	soviet::Tokenizer tokenizer;
	tokenizer.tokenize(line);

	soviet::Parser parser{tokenizer.getTokens()};

	std::shared_ptr<soviet::Node> tree;
	try {
		tree = parser.parse();
	} catch (const soviet::Error& error) {
		if (error.type == soviet::ErrorType::no_tokens) {
			getline(evaluator, line);
			return;
		}
	}

	try {
		evaluator.evaluate(tree);
	} catch (const std::exception& error) {
		std::cerr << "[oostszeszenie cos poszlo nie tak] " << error.what() << std::endl;
	}
}

int main(int argc, char** argv) {
	if (argc == 1) { // REPL
		std::cout << "wellCUM to soviet 2.0 REPL (Really Epic Program Launcher)" << std::endl
			<< std::endl;

		soviet::Evaluator evaluator;
		while (true) {
			getline(evaluator);
		}
	} else { // argc >= 2 => file
		std::ifstream file{argv[1]};
		if (!file.is_open()) {
			std::cerr << "no i dupa! cos sie zesralo z plikiem" << std::endl;
			return 1;
		}

		soviet::Evaluator evaluator;
		while (!file.eof()) {
			getfileline(evaluator, file);
		}
	}
}
