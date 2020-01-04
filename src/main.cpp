#include <iostream>
#include <functional>
#include "constant.hpp"
#include "variable.hpp"
#include "matrix.hpp"
#include "helper.hpp"
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

    auto* T = Matrix::square({
        _(1.0f), _(0.0f), _(0.0f), _("p_x"),
        _(0.0f), _(1.0f), _(0.0f), _("p_y"),
        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
    });

    auto* R = Matrix::square({
        _("\\cos(r)"),   _("\\sin(r)"), _(0.0f), _(0.0f),
        -*_("\\sin(r)"), _("\\cos(r)"), _(0.0f), _(0.0f),
        _(0.0f),         _(0.0f),       _(1.0f), _(0.0f),
        _(0.0f),         _(0.0f),       _(0.0f), _(1.0f)
    });

    auto* TR = *T->copy() * R->copy();
    auto* RT = *T->copy() * R->copy();

    std::cout << "T*R: " << TR->format(latex) << std::endl;
    std::cout << "R*T: " << RT->format(latex) << std::endl;

    delete T;
    delete R;
    delete TR;
    delete RT;

    return 0;
}