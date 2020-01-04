//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "product.hpp"

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
