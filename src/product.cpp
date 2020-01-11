//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "product.hpp"
#include "constant.hpp"
#include "variable.hpp"
#include "invalid-expression.hpp"
#include "sum.hpp"

Product::Product() : Symbol{}, mFactors{} {}

Product::Product(Symbol *first, Symbol *second) : Symbol{}, mFactors{} {
    mFactors.push_back(first);
    mFactors.push_back(second);
}

Product::~Product() {
    for (auto& factor : mFactors) {
        delete factor;
    }
}

Symbol *Product::copy() const {
    auto* copy = new Product{};
    for (auto& factor : mFactors) {
        copy->mFactors.push_back(factor->copy());
    }
    return copy;
}

Symbol* Product::copyRange(int beginIncl, int endIncl) const {
    if (beginIncl < 0 || beginIncl > endIncl || endIncl >= mFactors.size()) {
        throw std::invalid_argument(
            "Can't copy range [" + std::to_string(beginIncl) +
            ", " + std::to_string(endIncl) + "] of product with " +
            std::to_string(mFactors.size()) + " factors.");
    }

    if (beginIncl == endIncl) {
        return mFactors[beginIncl]->copy();
    }

    auto* copy = new Product{};
    for (int i = beginIncl; i <= endIncl; i++) {
        copy->mFactors.push_back(mFactors[i]->copy());
    }

    return copy;
}

Symbol *Product::negate() {
    bool hasConstantFactor = false;
    bool hasVariableFactor = false;
    for (auto& factor : mFactors) {
        if (auto* constant = dynamic_cast<Constant*>(factor)) {
            hasConstantFactor = true;
            if (constant->getValue() < 0.0f) {
                constant->negate();
                return this;
            }
        }

        if (auto* variable = dynamic_cast<Variable*>(factor)) {
            hasVariableFactor = true;
            if (variable->getQuantity() < 0.0f) {
                variable->negate();
                return this;
            }
        }
    }

    if (hasConstantFactor || hasVariableFactor) {
        for (auto& factor : mFactors) {
            if (auto *constant = dynamic_cast<Constant*>(factor)) {
                constant->negate();
                return this;
            }

            if (auto* variable = dynamic_cast<Variable*>(factor)) {
                variable->negate();
                return this;
            }
        }
    }

    for (auto& factor : mFactors) {
        factor->negate();
        return this;
    }

    delete this;
    return new Constant{-1.0f};
}

Symbol *Product::operator+(Symbol *other) {
    // TODO: Optimize expressions like: a*b + a*b = 2a*b
    return new Sum(this, other);
}

Symbol *Product::operator-(Symbol *other) {
    // TODO: Optimize expressions like: a*b - a*b = 0
    return new Sum(this, other->negate());
}

Symbol *Product::operator*(Symbol *other) {
    mFactors.push_back(other);
    return this;
}

Symbol *Product::operator/(Symbol *other) {

    // TODO: Check if any factor is equivalent to other, and in that case,
    //  delete that.

    throw InvalidExpression();
}

bool Product::isConstant() const {
    for (auto& factor : mFactors) {
        if (!factor->isConstant()) return false;
    }
    return true;
}

bool Product::isZero() const {
    for (auto& factor : mFactors) {
        if (factor->isZero()) return true;
    }
    return false;
}

int Product::getColumns() const {
    int cols, rows;
    getDimensions(cols, rows);
    return cols;
}

int Product::getRows() const {
    int cols, rows;
    getDimensions(cols, rows);
    return rows;
}

void Product::getDimensions(int &cols, int &rows) const {
    if (mFactors.empty()) throw InvalidExpression();
    cols = mFactors[0]->getColumns();
    rows = mFactors[0]->getRows();
    for (Factors::size_type i = 1; i < mFactors.size(); i++) {
        if (cols == 1 && rows == 1) {
            cols = mFactors[i]->getColumns();
            rows = mFactors[i]->getRows();

        } else if (mFactors[i]->isScalar()) {
            // Do nothing since dimensions doesn't change.

        } else if (cols == mFactors[i]->getRows()) {
            cols = mFactors[i]->getColumns();

        } else throw InvalidExpression(); // Dimensions does not match.
    }
}

std::set<std::string> Product::findUndefined() {
    std::set<std::string> undefined;
    for (auto & factors : mFactors) {
        auto termUndefined = factors->findUndefined();
        for (auto& u : termUndefined) {
            undefined.insert(u);
        }
    }
    return undefined;
}

std::string Product::format(const Formatter &formatter) const {
    if (mFactors.size() < 2) throw InvalidExpression();

    std::string str = formatter.times(
        mFactors[0]->format(formatter),
        mFactors[1]->format(formatter));

    for (Factors::size_type i = 2; i < mFactors.size(); i++) {
        str = formatter.times(str, mFactors[i]->format(formatter));
    }

    return str;
}

const Symbol *Product::get(int factor) const {
    return mFactors[factor];
}

int Product::getFactors() const {
    return mFactors.size();
}

void Product::setFactor(int index, Symbol* factor) {
    if (index == mFactors.size()) {
        mFactors.push_back(factor);
    } else {
        delete mFactors[index];
        mFactors[index] = factor;
    }
}

void Product::deleteFactor(int index) {
    delete mFactors[index];
    mFactors.erase(mFactors.begin() + index);
}

Symbol *Product::replace(const std::function<bool(const Symbol *)> &predicate,
                         const std::function<Symbol *(Symbol *)> &mapper) {

    for (auto& factor : mFactors) {
        if (predicate(factor)) {
            factor = mapper(factor);
        }
    }

    return this;
}
