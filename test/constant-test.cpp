//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "gtest/gtest.h"
#include "../src/constant.hpp"

TEST(constant, createConstant) {
    auto* constant = new Constant{5.0f};
    EXPECT_FLOAT_EQ(constant->getValue(), 5.0f);
    delete constant;
}

TEST(constant, sumTwoTerms) {
    auto* term1 = new Constant{5.0f};
    auto* term2 = new Constant{7.5f};
    auto* term3 = dynamic_cast<Constant*>(*term1 + term2);
    EXPECT_FLOAT_EQ(term3->getValue(), 12.5f);
    delete term3;
}

TEST(constant, subtractTwoTerms) {
    auto* term1 = new Constant{5.0f};
    auto* term2 = new Constant{7.5f};
    auto* term3 = dynamic_cast<Constant*>(*term1 - term2);
    EXPECT_FLOAT_EQ(term3->getValue(), -2.5f);
    delete term3;
}

TEST(constant, multiplyTwoTerms) {
    auto* term1 = new Constant{5.0f};
    auto* term2 = new Constant{7.5f};
    auto* term3 = dynamic_cast<Constant*>(*term1 * term2);
    EXPECT_FLOAT_EQ(term3->getValue(), 37.5f);
    delete term3;
}

TEST(constant, divideTwoTerms) {
    auto* term1 = new Constant{20.0f};
    auto* term2 = new Constant{2.5f};
    auto* term3 = dynamic_cast<Constant*>(*term1 / term2);
    EXPECT_FLOAT_EQ(term3->getValue(), 8.0f);
    delete term3;
}