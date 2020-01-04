//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <algorithm>
#include <utility>
#include "sum.hpp"
#include "variable.hpp"
#include "constant.hpp"
#include "invalid-expression.hpp"
#include "product.hpp"

Symbol *Sum::copy() const {
    if (mTerms.size() < 2) throw InvalidExpression();
    auto* sum = new Sum(mTerms[0]->copy(), mTerms[1]->copy());
    for (Terms::size_type i = 2; i < mTerms.size(); i++) {
        sum->mTerms.push_back(mTerms[i]->copy());
    }
    return sum;
}

Symbol *Sum::negate() {
    for (auto & mTerm : mTerms) {
        mTerm->negate();
    }
    return this;
}

Symbol *Sum::operator+(Symbol *other) {
    assertSameDimensions(other);

    auto otherUndefined = other->findUndefined();
    for (auto & term : mTerms) {
        auto termUndefined = term->findUndefined();
        if (otherUndefined == termUndefined) {
            auto* result = *term + other;
            if (result != nullptr) {
                term = result;
                return this;
            }
        }
    }

    mTerms.push_back(other);
    return this;
}

Symbol *Sum::operator-(Symbol *other) {
    assertSameDimensions(other);
    return assertNotNull(*this + other->negate());
}

Symbol *Sum::operator*(Symbol *other) {
    assertSameDimensions(other);

    if (auto* otherSum = dynamic_cast<Sum*>(other)) {
        Terms terms;
        for (auto& leftTerm : mTerms) {
            for (auto& rightTerm : otherSum->mTerms) {
                auto* result = *leftTerm->copy() * rightTerm->copy();
                if (result == nullptr) throw InvalidExpression();

                // First see if this term eliminates any previous term
                bool found = false;
                for (Terms::size_type i = 0; i < terms.size(); i++) {

                    auto* left  = terms[i]->copy();
                    auto* right = result->copy();
                    auto* sum = *left + right;

                    if (sum->isZero()) {
                        // The two terms eliminate each other.
                        delete sum;
                        delete terms[i];
                        terms.erase(terms.begin() + i);
                        found = true;
                        break;

                    } else if (!dynamic_cast<Sum*>(sum)) {
                        // This did not create a new level of summation, which is good.
                        delete terms[i];
                        terms[i] = sum;
                        found = true;
                        break;

                    } else {
                        // No good, the sum is just moved down one level.
                        delete sum;
                    }
                }

                if (found) delete result;
                else terms.push_back(result);
            }
        }

        // TODO: Optimize terms (A+B)*(A+B) = A^2+2AB+B^2

        delete other;
        delete this;


        return new Sum(terms);
    }

    for (auto & term : mTerms) {
        auto* result = *term * other->copy();
        if (result == nullptr) throw InvalidExpression();
        term = result;
    }
    delete other;
    return this;
}

Symbol *Sum::operator/(Symbol *other) {
    throw InvalidExpression(); // TODO: Not yet implemented.
}

bool Sum::isConstant() const {
    for (auto & term : mTerms) {
        if (!term->isConstant()) return false;
    }
    return true;
}

bool Sum::isZero() const {
    switch (mTerms.size()) {
        case 0: return true;
        case 1: return mTerms[0]->isZero();
        case 2: {
            auto* left  = mTerms[0]->copy();
            auto* right = mTerms[1]->copy();
            auto* diff = *left - right;
            bool zero = (dynamic_cast<Sum*>(diff)) ? false : diff->isZero();
            delete diff;
            return zero;
        }
    }

    Terms termsCopy {};
    for (auto mTerm : mTerms) {
        bool found = false;
        for (Terms::size_type i = 0; i < termsCopy.size(); i++) {
            auto* left  = mTerm->copy();
            auto* right = termsCopy[i]->copy();
            auto* diff = *left - right;
            bool zero = (dynamic_cast<Sum*>(diff)) ? false : diff->isZero();
            delete diff;
            if (zero) {
                found = true;
                delete termsCopy[i];
                termsCopy.erase(termsCopy.begin() + i);
                break;
            }
        }

        if (!found) {
            termsCopy.push_back(mTerm->copy());
        }
    }

    if (termsCopy.empty()) {
        return true;
    } else {
        for (auto& t : termsCopy) {
            delete t;
        }
        return false;
    }
}

int Sum::getColumns() const {
    int cols, rows;
    getDimensions(cols, rows);
    return cols;
}

int Sum::getRows() const {
    int cols, rows;
    getDimensions(cols, rows);
    return rows;
}

void Sum::getDimensions(int &cols, int &rows) const {
    cols = 1;
    rows = 1;
    for (auto& term : mTerms) {
        if (!term->isScalar()) {
            int termCols = term->getColumns();
            int termRows = term->getRows();

            if ((cols == 1 && rows == 1)
            || ((cols == termCols) && (rows == termRows))) {
                cols = termCols;
                rows = termRows;
            } else {
                throw InvalidExpression(); // Dimensions does not match
            }
        }
    }
}

std::set<std::string> Sum::findUndefined() {
    std::set<std::string> undefined;
    for (auto & term : mTerms) {
        auto termUndefined = term->findUndefined();
        for (auto& u : termUndefined) {
            undefined.insert(u);
        }
    }
    return undefined;
}

Sum::Sum(Symbol *first, Symbol *second) : Symbol{}, mTerms{} {
    mTerms.push_back(first);
    mTerms.push_back(second);
}

Sum::Sum(Sum::Terms terms) : Symbol{}, mTerms{std::move(terms)} {}

Sum::~Sum() {
    for (auto & term : mTerms) {
        delete term;
    }
}

bool Sum::hasMinusSign(const Symbol *symbol) const {
    if (const auto* constant = dynamic_cast<const Constant*>(symbol)) {
        return constant->getValue() < 0.0f;
    } else if (const auto* variable = dynamic_cast<const Variable*>(symbol)) {
        return variable->getQuantity() < 0.0f;
    } else if (const auto* product = dynamic_cast<const Product*>(symbol)) {
        return hasMinusSign(product->get(0));
    }
    return false;
}

std::string Sum::format(const Formatter &formatter) const {
    if (mTerms.size() <= 1) throw InvalidExpression();

    std::string str;
    if (hasMinusSign(mTerms[1])) {
        auto* copy = mTerms[1]->copy()->negate();
        str = formatter.minus(
            mTerms[0]->format(formatter),
            copy->format(formatter));
        delete copy;
    } else {
        str = formatter.plus(
            mTerms[0]->format(formatter),
            mTerms[1]->format(formatter));
    }

    for (Terms::size_type i = 2; i < mTerms.size(); i++) {
        if (hasMinusSign(mTerms[i])) {
            auto* copy = mTerms[i]->copy()->negate();
            str = formatter.minus(str, copy->format(formatter));
            delete copy;
        } else {
            str = formatter.plus(str, mTerms[i]->format(formatter));
        }
    }

    return formatter.paranthesis(str);
}
