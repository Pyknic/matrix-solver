//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "symbol.hpp"
#include "invalid-expression.hpp"
#include "constant.hpp"

void Symbol::assertSameDimensions(const Symbol *other) const {
    if (getRows() != other->getRows() || getColumns() != other->getColumns()) {
        throw InvalidExpression();
    }
}

Symbol* Symbol::assertNotNull(Symbol* symbol) const {
    if (symbol == nullptr) throw InvalidExpression();
    return symbol;
}

Symbol *Symbol::operator+(Symbol::value_t other) {
    return *this + new Constant(other);
}
