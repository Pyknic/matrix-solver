//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <vector>
#include "symbol.hpp"

class Sum : public Symbol {
public:
    Sum(Symbol* first, Symbol* second);

    ~Sum() override;

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
    void getDimensions(int& cols, int& rows) const;

    bool hasMinusSign(const Symbol* symbol) const;

    typedef std::vector<Symbol*> Terms;
    explicit Sum(Terms  terms);

    Terms mTerms;
};


