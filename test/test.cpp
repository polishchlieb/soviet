#ifdef RUN_TESTS
#include <catch.hpp>

#include "../tokenizer/Tokenizer.hpp"
#include "../parser/Parser.hpp"
#include "../evaluator/evaluator.hpp"
#include "../evaluator/valueCast.hpp"

TEST_CASE("Adds two numbers") {
    soviet::Tokenizer tokenizer;
    tokenizer.tokenize("2 + 2");

    soviet::Parser parser{ tokenizer.getTokens() };
    const auto tree = parser.parse();

    soviet::Evaluator evaluator;
    const auto value = evaluator.evaluate(tree);

    REQUIRE(valueCast<soviet::NumberValue>(value)->value == 4);
}
#endif