//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include "symbol.hpp"

class Variable : public Symbol {
public:
    explicit Variable(std::string name);
    void setQuantity(Symbol::value_t quantity);

    Symbol::value_t getQuantity() const;

    void setExponent(Symbol::value_t exponent);

    Symbol::value_t getExponent() const;

    ~Variable() override;

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
    const std::string mName;
    Symbol::value_t mQuantity;
    Symbol::value_t mExponent;
};


