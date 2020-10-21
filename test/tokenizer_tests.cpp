#include <gtest/gtest.h>
#include "../tokenizer/TestTokenizer.hpp"

class TokenizerTest : public ::testing::Test {
protected:
    soviet::TestTokenizer t;
};

TEST_F(TokenizerTest, TokenizesAddOperation) {
    t.addLine("2 + 3");
    const auto value = t.getTokens();

    EXPECT_EQ(value.size(), 3);

    EXPECT_EQ(value[0].type, soviet::TokenType::number);
    EXPECT_EQ(value[0].value, "2");

    EXPECT_EQ(value[1].type, soviet::TokenType::add_op);
    EXPECT_EQ(value[1].value, "+");

    EXPECT_EQ(value[2].type, soviet::TokenType::number);
    EXPECT_EQ(value[2].value, "3");
}

TEST_F(TokenizerTest, TokenizesMoreComplexExpressions) {
    t.addLine("object.function = (a, b) -> (1 + sqrt(2)) / (a - b)");
    const auto value = t.getTokens();

    EXPECT_EQ(value.size(), 24);

    EXPECT_EQ(value[0].type, soviet::TokenType::name);
    EXPECT_EQ(value[0].value, "object");

    EXPECT_EQ(value[1].type, soviet::TokenType::dot);

    EXPECT_EQ(value[2].type, soviet::TokenType::name);
    EXPECT_EQ(value[2].value, "function");

    EXPECT_EQ(value[3].type, soviet::TokenType::equals_op);

    EXPECT_EQ(value[4].type, soviet::TokenType::open_bracket);

    EXPECT_EQ(value[5].type, soviet::TokenType::name);
    EXPECT_EQ(value[5].value, "a");

    EXPECT_EQ(value[6].type, soviet::TokenType::comma);

    EXPECT_EQ(value[7].type, soviet::TokenType::name);
    EXPECT_EQ(value[7].value, "b");

    EXPECT_EQ(value[8].type, soviet::TokenType::close_bracket);

    EXPECT_EQ(value[9].type, soviet::TokenType::arrow);

    EXPECT_EQ(value[10].type, soviet::TokenType::open_bracket);

    EXPECT_EQ(value[11].type, soviet::TokenType::number);
    EXPECT_EQ(value[11].value, "1");

    EXPECT_EQ(value[12].type, soviet::TokenType::add_op);

    EXPECT_EQ(value[13].type, soviet::TokenType::name);
    EXPECT_EQ(value[13].value, "sqrt");

    EXPECT_EQ(value[14].type, soviet::TokenType::open_bracket);

    EXPECT_EQ(value[15].type, soviet::TokenType::number);
    EXPECT_EQ(value[15].value, "2");

    EXPECT_EQ(value[16].type, soviet::TokenType::close_bracket);
    EXPECT_EQ(value[17].type, soviet::TokenType::close_bracket);

    EXPECT_EQ(value[18].type, soviet::TokenType::div_op);

    EXPECT_EQ(value[19].type, soviet::TokenType::open_bracket);

    EXPECT_EQ(value[20].type, soviet::TokenType::name);
    EXPECT_EQ(value[20].value, "a");

    EXPECT_EQ(value[21].type, soviet::TokenType::sub_op);
    EXPECT_EQ(value[21].value, "-");

    EXPECT_EQ(value[22].type, soviet::TokenType::name);
    EXPECT_EQ(value[22].value, "b");

    EXPECT_EQ(value[23].type, soviet::TokenType::close_bracket);
}