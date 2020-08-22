#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "parser/dumpNode.hpp"
#include "evaluator/evaluator.hpp"
#include "evaluator/dumpValue.hpp"

int main() {
    tokenizer::Tokenizer t;
    parser::Parser p;
    evaluator::Evaluator e;

    while (true) {
        std::cout << "Input: ";
        std::string input;
        std::getline(std::cin, input);
        std::cout << "---------------------" << std::endl;

        if (input == "exit") {
            break;
        }

        auto tokens = t.tokenize(input);
        std::cout << "Tokenizer output:" << std::endl;
        for (const auto& token : tokens) {
            std::cout << "(" << tokenizer::dumpTokenType(token.type) << ") " << token.value << std::endl;
        }
        std::cout << "---------------------" << std::endl;

        auto rootNode = p.parse(tokens);
        std::cout << "Parser output:" << std::endl;
        parser::dump(rootNode);
        std::cout << "---------------------" << std::endl;

        std::cout << "Evaluator output:" << std::endl;
        const auto value = e.evaluate(rootNode);
        std::cout << evaluator::dumpValue(value) << std::endl << std::endl;
    }
}

