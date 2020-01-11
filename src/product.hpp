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

    Symbol* copyRange(int beginIncl, int endIncl) const;

    Symbol *negate() override;

    Symbol *operator+(Symbol *other) override;

    Symbol *operator-(Symbol *other) override;

    Symbol *operator*(Symbol *other) override;

    Symbol *operator/(Symbol *other) override;

    Symbol *replace(const std::function<bool(const Symbol *)> &predicate,
                    const std::function<Symbol *(Symbol *)> &mapper) override;

    std::string format(const Formatter &formatter) const override;

    bool isConstant() const override;

    bool isZero() const override;

    int getColumns() const override;

    int getRows() const override;

    int getFactors() const;

    void setFactor(int index, Symbol* factor);

    void deleteFactor(int index);

    const Symbol* get(int factor) const;

    std::set<std::string> findUndefined() override;

private:
    explicit Product();

    void getDimensions(int& cols, int& rows) const;

    typedef std::vector<Symbol*> Factors;
    Factors mFactors;
};


