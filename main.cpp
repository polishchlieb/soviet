#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "evaluator/evaluator.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "tokenizer/InlineTokenizer.hpp"

#include <filesystem>
#include "evaluator/moduleLoader.hpp"

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

//    soviet::Scope sinScope;
//    sinScope.variables.insert({
//        "sin",
//        std::make_shared<soviet::FunctionValue>([](
//            const std::vector<std::shared_ptr<soviet::Value>>& args
//        ) {
//            if (args.size() != 1)
//                throw soviet::EvaluateError("");
//            if (args[0]->type != soviet::ValueType::NumberValue)
//                throw soviet::EvaluateError("");
//
//            const auto value = soviet::valueCast<soviet::NumberValue>(args[0])->value;
//
//            return std::make_shared<soviet::NumberValue>(std::sin(value));
//        })
//    });
//    evaluator.currentContext[0].merge(sinScope);
    for (const auto& entry : std::filesystem::directory_iterator("soviet-lib/")) {
        auto module = loadModule(entry.path().c_str());
        evaluator.currentContext[0].merge(module);
    }

    for (;;) {
        try {
            const auto rootNode = parser.parse();
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

