//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <vector>
#include "symbol.hpp"

class Product : public Symbol {
public:
    explicit Product(Symbol* first, Symbol* second);

    ~Product() override;

    Symbol *copy() const override;

    Symbol *negate() override;

    Symbol *operator+(Symbol *other) override;

    Symbol *operator-(Symbol *other) override;

    Symbol *operator*(Symbol *other) override;

    Symbol *operator/(Symbol *other) override;

    bool isConstant() const override;

    int getColumns() const override;

    int getRows() const override;

    std::set<std::string> findUndefined() override;

private:
    typedef std::vector<Symbol*> Factors;
    Factors mFactors;
};


