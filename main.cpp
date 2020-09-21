#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "evaluator/evaluator.hpp"
#include "evaluator/dumpValue.hpp"

#ifdef DEBUG
#include "tokenizer/dumpTokens.hpp"
#include "parser/dumpNode.hpp"
#endif

int main() {
    soviet::Tokenizer tokenizer;
    soviet::Parser parser;
    soviet::Evaluator evaluator;

    while (true) {
        std::string input;

#ifdef DEBUG
        std::cout << "Input: ";
        std::getline(std::cin, input);
        std::cout << "---------------------" << std::endl;
#else
        std::cout << "> ";
        std::getline(std::cin, input);
#endif

        if (input == "exit") {
            break;
        }

#ifdef DEBUG
        try {
            tokenizer.tokenize(input);
            auto it = tokenizer.getIterator();
            std::cout << "Tokenizer output:" << std::endl;
            it.dump();
            std::cout << "---------------------" << std::endl;

            auto rootNode = parser.parse(std::move(it));
            std::cout << "Parser output:" << std::endl;
            soviet::dump(rootNode);
            std::cout << "---------------------" << std::endl;

            std::cout << "Evaluator output:" << std::endl;
            const auto value = evaluator.evaluate(rootNode);
            std::cout << soviet::dumpValue(value) << std::endl << std::endl;
        } catch (soviet::Error& e) {
            e.print();
        }

        tokenizer.clear();
#else
        try {
            tokenizer.tokenize(input);
            const auto rootNode = parser.parse(tokenizer.getIterator());
            const auto value = evaluator.evaluate(rootNode);
            std::cout << soviet::dumpValue(value) << std::endl;
        } catch (soviet::Error& e) {
            e.print();
        }

        tokenizer.clear();
#endif
    }
}

