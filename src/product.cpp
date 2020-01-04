//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "product.hpp"
#include "invalid-expression.hpp"

Product::Product(Symbol *first, Symbol *second) : mFactors{} {
    mFactors.push_back(first);
    mFactors.push_back(second);
}

Product::~Product() {
    for (auto& factor : mFactors) {
        delete factor;
    }
}

Symbol *Product::copy() const {
    return nullptr;
}

Symbol *Product::negate() {
    return nullptr;
}

Symbol *Product::operator+(Symbol *other) {
    return nullptr;
}

Symbol *Product::operator-(Symbol *other) {
    return nullptr;
}

Symbol *Product::operator*(Symbol *other) {
    return nullptr;
}

Symbol *Product::operator/(Symbol *other) {
    return nullptr;
}

bool Product::isConstant() const {
    return false;
}

int Product::getColumns() const {
    return 0;
}

int Product::getRows() const {
    return 0;
}

std::set<std::string> Product::findUndefined() {
    return std::set<std::string>();
}

std::string Product::format(const Formatter &formatter) const {
    if (mFactors.size() <= 1) throw InvalidExpression();

    std::string str = formatter.times(
        mFactors[0]->format(formatter),
        mFactors[1]->format(formatter));

    for (Factors::size_type i = 2; i < mFactors.size(); i++) {
        str = formatter.times(str, mFactors[i]->format(formatter));
    }

    return str;
}
