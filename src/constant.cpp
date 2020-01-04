//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <cfloat>
#include <cmath>
#include "constant.hpp"
#include "sum.hpp"
#include "invalid-expression.hpp"

Symbol *Constant::negate() {
    mValue = -mValue;
    return this;
}

Symbol *Constant::operator+(Symbol *other) {
    if (isZero()) {
        delete this;
        return other;
    }

    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        mValue += otherConstant->mValue;
        delete otherConstant;
        return this;
    }

    return new Sum(this, other);
}

Symbol *Constant::operator-(Symbol *other) {
    if (isZero()) {
        delete this;
        return other->negate();
    }

    return assertNotNull(*this + other->negate());
}

Symbol *Constant::operator*(Symbol *other) {
    if (isZero()) {
        delete other;
        return this;
    } else if (isOne()) {
        delete this;
        return other;
    } else if (isMinusOne()) {
        delete this;
        return other->negate();
    }

    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        mValue *= otherConstant->mValue;
        delete other;
        return this;
    }

    // TODO: Add product class
    throw InvalidExpression();
}

Symbol *Constant::operator/(Symbol *other) {
    if (isZero()) {
        delete other;
        return this;
    }

    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        mValue /= otherConstant->mValue;
        delete other;
        return this;
    }
    // TODO: Add fractions class
    throw InvalidExpression();
}

Symbol *Constant::operator+(Symbol::value_t other) {
    mValue += other;
    return this;
}

Symbol *Constant::operator-(Symbol::value_t other) {
    mValue -= other;
    return this;
}

Symbol *Constant::operator*(Symbol::value_t other) {
    mValue *= other;
    return this;
}

Symbol *Constant::operator/(Symbol::value_t other) {
    mValue /= other;
    return this;
}

bool Constant::isConstant() const {
    return true;
}

int Constant::getColumns() const {
    return 1;
}

int Constant::getRows() const {
    return 1;
}

std::set<std::string> Constant::findUndefined() {
    return std::set<std::string>();
}

Constant::Constant(Symbol::value_t value) : mValue{value} {}

Constant& Constant::operator=(Symbol::value_t value) {
    mValue = value;
    return *this;
}

bool Constant::isOne() const {
    return fabsf(mValue - 1.0f) < FLT_EPSILON;
}

bool Constant::isZero() const {
    return fabsf(mValue) < FLT_EPSILON;
}

bool Constant::isMinusOne() const {
    return fabsf(mValue + 1.0f) < FLT_EPSILON;
}

Symbol::value_t Constant::getValue() const {
    return mValue;
}

Symbol *Constant::copy() const {
    return new Constant(mValue);
}

Constant::~Constant() = default;