//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <cmath>
#include <cfloat>
#include <utility>
#include "variable.hpp"
#include "sum.hpp"
#include "constant.hpp"
#include "product.hpp"
#include "invalid-expression.hpp"

Variable::Variable(std::string name)
    : Symbol{}, mName{std::move(name)}, mQuantity{1.0f}, mExponent{1.0f} {}

Variable::~Variable() = default;

void Variable::setQuantity(Symbol::value_t quantity) {
    mQuantity = quantity;
}

Symbol::value_t Variable::getQuantity() const {
    return mQuantity;
}

void Variable::setExponent(Symbol::value_t exponent) {
    mExponent = exponent;
}

Symbol::value_t Variable::getExponent() const {
    return mExponent;
}

Symbol *Variable::copy() const {
    auto* copy = new Variable{mName};
    copy->setQuantity(mQuantity);
    copy->setExponent(mExponent);
    return copy;
}

Symbol *Variable::negate() {
    mQuantity = -mQuantity;
    return this;
}

Symbol *Variable::operator+(Symbol *other) {
    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        if (otherConstant->isZero()) {
            delete other;
            return this;
        }
    }

    else if (auto* otherVariable = dynamic_cast<Variable*>(other)) {
        if (fabsf(otherVariable->mQuantity) < FLT_EPSILON) {
            delete other;
            return this;
        }

        if (fabsf(otherVariable->mExponent) < FLT_EPSILON) {
            Symbol* result = *this + new Constant{otherVariable->mQuantity};
            delete other;
            return result;
        }

        if (mName == otherVariable->mName
        &&  fabsf(mExponent - otherVariable->mExponent) < FLT_EPSILON) {
            // 2x^3 + 4x^3 = 6x^3
            mQuantity += otherVariable->mQuantity;
            delete other;
            return this;
        }
    }

    else if (auto* otherSum = dynamic_cast<Sum*>(other)) {
        return *otherSum + this;
    }

    return new Sum(this, other);
}

Symbol *Variable::operator-(Symbol *other) {
    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        if (otherConstant->isZero()) {
            delete other;
            return this;
        }
    }

    else if (auto* otherVariable = dynamic_cast<Variable*>(other)) {
        if (fabsf(otherVariable->mQuantity) < FLT_EPSILON) {
            delete other;
            return this;
        }

        if (fabsf(otherVariable->mExponent) < FLT_EPSILON) {
            Symbol* result = *this + new Constant{-otherVariable->mQuantity};
            delete other;
            return result;
        }

        if (mName == otherVariable->mName
        &&  fabsf(mExponent - otherVariable->mExponent) < FLT_EPSILON) {
            // 2x^3 - 4x^3 = -2x^3
            mQuantity -= otherVariable->mQuantity;
            delete other;
            return this;
        }
    }

    else if (auto* otherSum = dynamic_cast<Sum*>(other)) {
        return *otherSum + this;
    }

    return new Sum(this, other->negate());
}

Symbol *Variable::operator*(Symbol *other) {
    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        if (otherConstant->isZero()) {
            delete this;
            return other;
        } else if (otherConstant->isOne()) {
            delete other;
            return this;
        }  else if (otherConstant->isMinusOne()) {
            delete other;
            return this->negate();
        }

        mQuantity *= otherConstant->getValue();
        delete other;
        return this;
    }

    if (auto* otherVariable = dynamic_cast<Variable*>(other)) {
        if (fabsf(otherVariable->mQuantity) < FLT_EPSILON) {
            delete other;
            delete this;
            return new Constant{0.0f};
        }

        if (fabsf(otherVariable->mExponent) < FLT_EPSILON) {
            mQuantity *= otherVariable->getQuantity();
            delete other;
            return this;
        }

        if (mName == otherVariable->mName) {
            mQuantity *= otherVariable->mQuantity;
            mExponent += otherVariable->getExponent();

            delete other;

            if (fabsf(mQuantity) < FLT_EPSILON) {
                delete this;
                return new Constant(0.0f);
            }

            if (fabsf(mExponent) < FLT_EPSILON) {
                delete this;
                return new Constant(mQuantity);
            }

            return this;
        }

        float quantity = mQuantity * otherVariable->mQuantity;
        otherVariable->mQuantity = 1.0f;
        mQuantity = 1.0f;

        Symbol* result = new Product(this, other);
        if (fabsf(quantity - 1.0f) >= FLT_EPSILON) {
            result = *new Constant(quantity) * result;
        }

        return result;
    }

    if (auto* otherProduct = dynamic_cast<Product*>(other)) {
        return *otherProduct * this;
    }

    if (auto* otherSum = dynamic_cast<Sum*>(other)) {
        return *otherSum * this;
    }

    throw InvalidExpression();
}

Symbol *Variable::operator/(Symbol *other) {
    if (auto* otherConstant = dynamic_cast<Constant*>(other)) {
        if (otherConstant->isZero()) {
            throw InvalidExpression();
        } else if (otherConstant->isOne()) {
            delete other;
            return this;
        }  else if (otherConstant->isMinusOne()) {
            delete other;
            return this->negate();
        }

        mQuantity /= otherConstant->getValue();
        delete other;
        return this;
    }

    if (auto* otherVariable = dynamic_cast<Variable*>(other)) {
        if (fabsf(otherVariable->mQuantity) < FLT_EPSILON) {
            throw InvalidExpression(); // Since this is division
        }

        if (fabsf(otherVariable->mExponent) < FLT_EPSILON) {
            mQuantity /= otherVariable->getQuantity();
            delete other;
            return this;
        }

        if (mName == otherVariable->mName) {
            mQuantity /= otherVariable->mQuantity;
            mExponent -= otherVariable->getExponent();

            delete other;

            if (fabsf(mQuantity) < FLT_EPSILON) {
                delete this;
                return new Constant(0.0f);
            }

            if (fabsf(mExponent) < FLT_EPSILON) {
                delete this;
                return new Constant(mQuantity);
            }

            return this;
        }

        throw InvalidExpression(); // TODO: Support fractions
    }

    // TODO: Division by product should be performed by dividing by one element at a time.

    throw InvalidExpression();
}

bool Variable::isConstant() const {
    return false;
}

int Variable::getColumns() const {
    return 1;
}

int Variable::getRows() const {
    return 1;
}

std::set<std::string> Variable::findUndefined() {
    auto set = std::set<std::string>();
    set.insert(mName);
    return set;
}

std::string Variable::format(const Formatter &formatter) const {
    Constant quantityConstant {mQuantity};
    Constant exponentConstant {mExponent};

    std::string inner;
    if (fabsf(mExponent) < FLT_EPSILON) { // Exponent == 0
        return formatter.constant(&quantityConstant);

    } else if (fabsf(mExponent - 1.0f) < FLT_EPSILON) { // Exponent == 1
        inner = formatter.unknown(mName);

    } else {
        inner = formatter.power(
            formatter.unknown(mName),
            formatter.constant(&exponentConstant)
        );
    }

    std::string outer;
    if (fabsf(mQuantity - 1.0f) < FLT_EPSILON) { // Quantity == 1
        outer = inner;

    } else if (fabsf(mQuantity) < FLT_EPSILON) { // Quantity == 0
        Constant zero {0.0f};
        outer = formatter.constant(&zero);

    } else if (fabsf(mQuantity + 1.0f) < FLT_EPSILON) { // Quantity == -1
        outer = formatter.paranthesis(formatter.negate(inner));

    } else {
        outer = formatter.times(
            formatter.constant(&quantityConstant),
            inner
        );
    }

    return outer;
}

bool Variable::isZero() const {
    return mQuantity == 0;
}

Symbol *Variable::replace(const std::function<bool(const Symbol *)> &predicate,
                          const std::function<Symbol *(Symbol *)> &mapper) {
    return this;
}
