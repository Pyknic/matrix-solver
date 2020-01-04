//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <algorithm>
#include <utility>
#include "sum.hpp"
#include "invalid-expression.hpp"

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
                terms.push_back(result);
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

int Sum::getColumns() const {
    return mTerms[0]->getColumns();
}

int Sum::getRows() const {
    return mTerms[0]->getRows();
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

std::string Sum::format(const Formatter &formatter) const {
    if (mTerms.size() <= 1) throw InvalidExpression();

    std::string str = formatter.times(
        mTerms[0]->format(formatter),
        mTerms[1]->format(formatter));

    for (Terms::size_type i = 2; i < mTerms.size(); i++) {
        str = formatter.times(str, mTerms[i]->format(formatter));
    }

    return formatter.paranthesis(str);
}
