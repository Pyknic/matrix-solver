#pragma once

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

class Symbol;
class Constant;
class Variable;
class Matrix;
class Product;
class Sum;

class Optimizer {
public:
    explicit Optimizer();

    virtual ~Optimizer();

    Symbol* optimize(Symbol* input) const;

private:
    Symbol* optimizeConstant(Constant* input) const;

    Symbol* optimizeVariable(Variable* input) const;

    Symbol* optimizeMatrix(Matrix* input) const;

    Symbol* optimizeProduct(Product* input) const;

    Symbol* optimizeSum(Sum* input) const;
};
