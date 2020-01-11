#include <cfloat>
#include <cmath>
#include "optimizer.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "symbol.hpp"
#include "constant.hpp"
#include "variable.hpp"
#include "matrix.hpp"
#include "product.hpp"
#include "sum.hpp"

Optimizer::Optimizer() = default;

Optimizer::~Optimizer() = default;

Symbol* Optimizer::optimize(Symbol* input) const {
    if (auto* constant = dynamic_cast<Constant*>(input)) {
        return optimizeConstant(constant);
    } else if (auto* variable = dynamic_cast<Variable*>(input)) {
        return optimizeVariable(variable);
    } else if (auto* matrix = dynamic_cast<Matrix*>(input)) {
        return optimizeMatrix(matrix);
    } else if (auto* product = dynamic_cast<Product*>(input)) {
        return optimizeProduct(product);
    } else if (auto* sum = dynamic_cast<Sum*>(input)) {
        return optimizeSum(sum);
    } else {
        throw std::invalid_argument("Input was a symbol of an undefined type.");
    }
}

Symbol* Optimizer::optimizeConstant(Constant* input) const {
    return input;
}

Symbol* Optimizer::optimizeVariable(Variable* input) const {
    if (input->getQuantity() == 0) {
        delete input;
        return new Constant{0.0f};
    } else if (input->getExponent() == 0) {
        float quantity = input->getQuantity();
        delete input;
        return new Constant{quantity};
    } else {
        return input; // TODO: Try to resolve variable?
    }
}

Symbol* Optimizer::optimizeMatrix(Matrix* input) const {
    for (int i = 0; i < input->getRows(); i++) {
        for (int j = 0; j < input->getColumns(); j++) {
            input->set(i, j, optimize(input->get(i, j)->copy()));
        }
    }

    return input;
}

Symbol* Optimizer::optimizeProduct(Product* input) const {
    // Optimize each factor
    for (int i = 0; i < input->getFactors(); i++) {
        auto* symbol = input->get(i)->copy();
        input->setFactor(i, optimize(symbol));
    }

    // Early exit for simple products
    if (input->getFactors() == 0) {
        delete input;
        return new Constant{1.0f};
    } else if (input->getFactors() == 1) {
        auto* onlyFactor = input->get(0)->copy();
        delete input;
        return onlyFactor;
    }

    // Collapse hierarchies of factors
    for (int i = 0; i < input->getFactors();) {
        if (auto* product = dynamic_cast<const Product*>(input->get(i))) {
            auto* collapsed = product->copy();
            if (i > 0) {
                auto* preceding = product->copyRange(0, i - 1);
                collapsed = *preceding * collapsed;
            }

            if (i < input->getFactors() - 1) {
                auto* succeeding = product->copyRange(i + 1, product->getFactors() - 1);
                collapsed = *collapsed * succeeding;
            }

            delete input;
            if (auto* collapsedProduct = dynamic_cast<Product*>(collapsed)) {
                input = collapsedProduct;
                i = 0;
                continue;
            } else {
                return optimize(collapsed);
            }
        }

        i++;
    }

    // Extract all the constant factors into one number
    float constantFactor = 1.0f;
    for (int i = 0; i < input->getFactors();) {
        auto* symbol = input->get(i);
        if (auto* constant = dynamic_cast<const Constant*>(symbol)) {
            constantFactor *= constant->getValue();
            input->deleteFactor(i);
            continue;
        } else if (auto* variable = dynamic_cast<const Variable*>(symbol)) {
            constantFactor *= variable->getQuantity();
            auto* newVariable = new Variable{variable->getName()};
            newVariable->setExponent(variable->getExponent());
            input->setFactor(i, newVariable);
        }

        i++;
    }

    if (fabsf(constantFactor) < FLT_EPSILON) {
        delete input;
        return new Constant{0.0f};
    }

    if (input->getFactors() == 0) {
        delete input;
        return new Constant{constantFactor};

    } else if (input->getFactors() == 1) {
        auto* factorCopy = input->get(0)->copy();
        factorCopy = *factorCopy * new Constant{constantFactor};
        if (dynamic_cast<Product*>(factorCopy)) {
            delete factorCopy;
        } else {
            delete input;
            return factorCopy;
        }
    }

    // There are at least 2 factors left
    if (input->getFactors() >= 2) {
        auto* newProduct = *input->get(0)->copy() * input->get(1)->copy();

        for (int i = 2; i < input->getFactors(); i++) {
            newProduct = *newProduct * input->get(i)->copy();
        }

        delete input;
        if (auto* newProductCasted = dynamic_cast<Product*>(newProduct)) {
            input = newProductCasted;
        } else {
            newProduct = optimize(newProduct);
            return (*new Constant{constantFactor}) * newProduct;
        }
    }

    return input;
}

Symbol* Optimizer::optimizeSum(Sum* input) const {
    return input;
}
