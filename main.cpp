#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "evaluator/evaluator.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "tokenizer/InlineTokenizer.hpp"

#include <fstream>

#ifdef DEBUG
#include "tokenizer/dumpTokens.hpp"
#include "parser/dumpNode.hpp"
#endif

static void runREPL() {
    std::cout << "soviet v2137.0.1 interactive coś" << std::endl
        << "praise thy schab" << std::endl << std::endl;

    soviet::Parser<soviet::InlineTokenizer> parser;
    soviet::Evaluator evaluator;

    for (;;) {
        try {
            const auto rootNode = parser.parse();
            soviet::dump(rootNode);
            const auto value = evaluator.evaluate(rootNode);
            std::cout << soviet::dumpValue(value) << std::endl;
        } catch (const soviet::Error& e) {
            e.print();
            parser.tokenizer.clear();
        }
    }
}

static void runFile(const char* fileName) {
    soviet::Parser<soviet::FileTokenizer> parser{fileName};
    try {
        parser.tokenizer.initFile();
    } catch (const soviet::FileReadError& e) {
        e.print();
    }

    soviet::Evaluator evaluator;

    try {
        while (!parser.tokenizer.isEmpty()) {
            const auto rootNode = parser.parse();
            evaluator.evaluate(rootNode);
        }
    } catch (const soviet::Error& e) {
        e.print();
    }
}

int main(int argc, char** argv) {
    switch (argc) {
        case 1: runREPL(); break;
        case 2: runFile(argv[1]); break;
        default: std::cout << "blyat" << std::endl;
    }
}

