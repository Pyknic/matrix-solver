//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include "symbol.hpp"
#include <vector>

class Matrix : public Symbol {
public:
    static Matrix* eye(int size);

    static Matrix* zero(int size);

    static Matrix* zero(int rows, int cols);

    static Matrix* square(std::initializer_list<Symbol::value_t> elements);

    static Matrix* square(std::initializer_list<Symbol*> elements);

    ~Matrix() override;

    Symbol* get(int row, int col) const;

    void set(int row, int col, Symbol* element);

    Symbol *copy() const override;

    Symbol *negate() override;

    Symbol *operator+(Symbol *other) override;

    Symbol *operator-(Symbol *other) override;

    Symbol *operator*(Symbol *other) override;

    Symbol *operator/(Symbol *other) override;

    std::string format(const Formatter &formatter) const override;

    bool isConstant() const override;

    bool isZero() const override;

    int getColumns() const override;

    int getRows() const override;

    std::set<std::string> findUndefined() override;

private:
    explicit Matrix(int rows, int cols);

    Matrix(const Matrix& prototype);

    const int mRows;
    const int mCols;
    std::vector<Symbol*> mElements;
};
