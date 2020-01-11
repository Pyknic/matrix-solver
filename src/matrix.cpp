#include <cmath>
#include "matrix.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "constant.hpp"
#include "invalid-expression.hpp"

Matrix::Matrix(int rows, int cols) :
    Symbol{}, mRows{rows}, mCols{cols}, mElements{} {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            mElements.push_back(new Constant{0.0f});
        }
    }
}

Matrix::Matrix(const Matrix &prototype) :
    Symbol{}, mRows{prototype.mRows}, mCols{prototype.mCols}, mElements{} {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            int idx = i * mCols + j;
            mElements.push_back(prototype.mElements[idx]->copy());
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            delete mElements[i * mCols + j];
        }
    }
}

Symbol *Matrix::copy() const {
    return new Matrix(*this);
}

Symbol *Matrix::get(int row, int col) const {
    return mElements[row * mCols + col];
}

void Matrix::set(int row, int col, Symbol *element) {
    delete mElements[row * mCols + col];
    mElements[row * mCols + col] = element;
}

Symbol *Matrix::negate() {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            int idx = i * mCols + j;
            mElements[idx]->negate();
        }
    }
    return this;
}

Matrix* Matrix::transpose() {
    auto* transposed = new Matrix{mCols, mRows};
    for (int i = 0; i < mRows; i++) {
        for (int j = i; j < mCols; j++) {
            const int idx = i * mCols + j;
            transposed->set(j, i, mElements[idx]->copy());
        }
    }
    delete this;
    return transposed;
}

Symbol *Matrix::operator+(Symbol *other) {
    if (other->isScalar()) {
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < mCols; j++) {
                int idx = i * mCols + j;
                mElements[idx] = *mElements[idx] + other->copy();
            }
        }
        delete other;
        return this;
    }

    if (other->getRows() != mRows || other->getColumns() != mCols) {
        throw InvalidExpression(); // Dimensions does not match
    }

    if (auto* otherMatrix = dynamic_cast<Matrix*>(other)) {
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < mCols; j++) {
                int idx = i * mCols + j;
                mElements[idx] = *mElements[idx] + otherMatrix->mElements[idx]->copy();
            }
        }
        delete other;
        return this;
    }

    throw InvalidExpression();
}

Symbol *Matrix::operator-(Symbol *other) {
    return *this + other->negate();
}

Symbol *Matrix::operator*(Symbol *other) {
    if (other->isScalar()) {
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < mCols; j++) {
                int idx = i * mCols + j;
                mElements[idx] = *mElements[idx] * other->copy();
            }
        }
        delete other;
        return this;
    }

    if (other->getRows() != mCols) {
        throw InvalidExpression(); // Dimensions does not match
    }

    if (auto* otherMatrix = dynamic_cast<Matrix*>(other)) {
        if (otherMatrix->getRows() != mCols) {
            throw std::invalid_argument( // TODO: better exception handling
                "Can't compute matrix multiplication between [" +
                std::to_string(mRows) + ", " +
                std::to_string(mCols) + "] and [" +
                std::to_string(otherMatrix->mRows) + ", " +
                std::to_string(otherMatrix->mCols) + "] matrices.");
        }

        auto* result = new Matrix(mRows, other->getColumns());

        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < other->getColumns(); j++) {
                int resIdx = i * other->getColumns() + j;

                for (int k = 0; k < mCols; k++) {
                    int leftIdx = i * mCols + k;
                    int rightIdx = k * other->getColumns() + j;

                    Symbol* left = mElements[leftIdx];
                    Symbol* right = otherMatrix->mElements[rightIdx];

                    if (left->isZero() || right->isZero()) continue;

                    result->mElements[resIdx] = *result->mElements[resIdx] +
                        (*left->copy() * right->copy());
                }
            }
        }

        delete this;
        delete other;
        return result;
    }

    throw InvalidExpression();
}

Symbol *Matrix::operator/(Symbol *other) {
    if (other->isScalar()) {
        for (int i = 0; i < mRows; i++) {
            for (int j = 0; j < mCols; j++) {
                int idx = i * mCols + j;
                mElements[idx] = *mElements[idx] / other->copy();
            }
        }
        delete other;
        return this;
    }

    throw InvalidExpression();
}

std::string Matrix::format(const Formatter &formatter) const {
    std::vector<std::string> elements;

    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            elements.push_back(get(i, j)->format(formatter));
        }
    }

    return formatter.matrix(mRows, mCols, elements);
}

bool Matrix::isConstant() const {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            if (!get(i, j)->isConstant()) return false;
        }
    }
    return true;
}

bool Matrix::isZero() const {
    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            if (!get(i, j)->isZero()) return false;
        }
    }
    return true;
}

int Matrix::getColumns() const {
    return mCols;
}

int Matrix::getRows() const {
    return mRows;
}

std::set<std::string> Matrix::findUndefined() {
    std::set<std::string> undefined;

    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            auto termUndefined = get(i, j)->findUndefined();
            for (auto& u : termUndefined) {
                undefined.insert(u);
            }
        }
    }

    return undefined;
}

Matrix* Matrix::eye(int size) {
    auto* matrix = new Matrix{size, size};

    for (int i = 0; i < size; i++) {
        matrix->set(i, i, new Constant{1.0f});
    }

    return matrix;
}

Matrix *Matrix::zero(int size) {
    return new Matrix{size, size};
}

Matrix *Matrix::zero(int rows, int cols) {
    return new Matrix{rows, cols};
}

Matrix* Matrix::square(std::initializer_list<Symbol::value_t> elements) {
    int size = (int) sqrt(elements.size());
    auto* matrix = new Matrix{size, size};
    auto it = elements.begin();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int idx = i * size + j;
            matrix->set(i, j, new Constant{*it});
            it++;
        }
    }
    if (it != elements.end()) {
        throw InvalidExpression(); // Invalid number of elements
    }
    return matrix;
}

Matrix* Matrix::square(std::initializer_list<Symbol*> elements) {
    int size = (int) sqrt(elements.size());
    auto* matrix = new Matrix{size, size};
    auto it = elements.begin();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int idx = i * size + j;
            matrix->set(i, j, *it);
            it++;
        }
    }
    if (it != elements.end()) {
        throw InvalidExpression(); // Invalid number of elements
    }
    return matrix;
}

Symbol *Matrix::replace(const std::function<bool(const Symbol*)> &predicate,
                        const std::function<Symbol*(Symbol*)> &mapper) {

    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            const int idx = i * mCols + j;
            if (predicate(mElements[idx])) {
                mElements[idx] = mapper(mElements[idx]);
            }
        }
    }

    return this;
}