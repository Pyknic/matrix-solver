//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <string>
#include <sstream>
#include "gtest/gtest.h"
#include "../src/parser.hpp"
#include "../src/constant.hpp"

TEST(constant, parseSimpleAddition) {
    Parser parser{};

    std::stringstream input {"answer = 1 + 2;"};
    EXPECT_TRUE(parser.parse(input));

    Symbol* result = parser.get("answer");

    EXPECT_FALSE(result == nullptr);
    EXPECT_TRUE(dynamic_cast<Constant*>(result));
    auto* constant = dynamic_cast<Constant*>(result);

    EXPECT_FLOAT_EQ(constant->getValue(), 3.0f);
}

TEST(constant, parseConstantExpression) {
    Parser parser{};

    std::stringstream input {"answer = 7.5(1 + 2);"};
    EXPECT_TRUE(parser.parse(input));

    Symbol* result = parser.get("answer");

    EXPECT_FALSE(result == nullptr);
    EXPECT_TRUE(dynamic_cast<Constant*>(result));
    auto* constant = dynamic_cast<Constant*>(result);

    EXPECT_FLOAT_EQ(constant->getValue(), 22.5f);
}