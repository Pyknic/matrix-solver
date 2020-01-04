//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include "symbol.hpp"

class Constant : public Symbol {
public:
    explicit Constant(Symbol::value_t value);

    ~Constant() override;

    Constant& operator=(Symbol::value_t value);

    Symbol *negate() override;

    Symbol *operator+(Symbol *other) override;

    Symbol *operator-(Symbol *other) override;

    Symbol *operator*(Symbol *other) override;

    Symbol *operator/(Symbol *other) override;

    Symbol *operator+(value_t other) override;

    Symbol *operator-(value_t other) override;

    Symbol *operator*(value_t other) override;

    Symbol *operator/(value_t other) override;

    bool isConstant() const override;

    int getColumns() const override;

    int getRows() const override;

    std::set<std::string> findUndefined() override;

    bool isOne() const;

    bool isZero() const;

    bool isMinusOne() const;

    Symbol::value_t getValue() const;

    Symbol *copy() const override;

private:
    Symbol::value_t mValue;
};


