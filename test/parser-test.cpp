//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <string>
#include <sstream>
#include "gtest/gtest.h"
#include "../src/parser.hpp"
#include "../src/variable.hpp"
#include "../src/constant.hpp"
#include "../src/matrix.hpp"

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

TEST(constant, parseMatrixExpression) {
    Parser parser{};

    std::stringstream input {};
    input << "A = [a,b;c,d];\n";
    input << "B = [1,0;0,1];\n";
    input << "C = A * B;";

    EXPECT_TRUE(parser.parse(input));

    Symbol* result = parser.get("C");

    EXPECT_FALSE(result == nullptr);
    EXPECT_TRUE(dynamic_cast<Matrix*>(result));
    auto* matrix = dynamic_cast<Matrix*>(result);

    EXPECT_TRUE(dynamic_cast<Variable*>(matrix->get(0, 0)));
    EXPECT_STREQ(dynamic_cast<Variable*>(matrix->get(0, 0))->getName().c_str(), "a");

    EXPECT_TRUE(dynamic_cast<Variable*>(matrix->get(0, 1)));
    EXPECT_STREQ(dynamic_cast<Variable*>(matrix->get(0, 1))->getName().c_str(), "b");

    EXPECT_TRUE(dynamic_cast<Variable*>(matrix->get(1, 0)));
    EXPECT_STREQ(dynamic_cast<Variable*>(matrix->get(1, 0))->getName().c_str(), "c");

    EXPECT_TRUE(dynamic_cast<Variable*>(matrix->get(1, 1)));
    EXPECT_STREQ(dynamic_cast<Variable*>(matrix->get(1, 1))->getName().c_str(), "d");
}