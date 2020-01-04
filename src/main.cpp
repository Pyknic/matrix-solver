#include <iostream>
#include "constant.hpp"
#include "variable.hpp"
#include "latex-formatter.hpp"

int main() {
    LatexFormatter latex;
    std::cout << "Hello, World!" << std::endl;

    Symbol* expr = new Constant(5);
    expr = *expr + *((*new Constant(3)) * new Variable("x")) * new Variable("x");

    std::cout << expr->format(latex) << std::endl;

    expr = *expr * expr->copy();
    std::cout << expr->format(latex) << std::endl;

    delete expr;
    return 0;
}