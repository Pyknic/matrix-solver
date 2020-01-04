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

    auto* A = Matrix::square({
        _("a_x"), _(0.0f),  _(0.0f), _(0.0f),
        _(0.0f),  _("a_y"), _(0.0f), _(0.0f),
        _(0.0f),  _(0.0f),  _(1.0f), _(0.0f),
        _(0.0f),  _(0.0f),  _(0.0f), _(1.0f)
    });

    auto* O = Matrix::square({
        _(1.0f), _(0.0f), _(0.0f), -*_("o_x"),
        _(0.0f), _(1.0f), _(0.0f), -*_("o_y"),
        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
    });

    auto* P = Matrix::square({
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

    auto* S = Matrix::square({
        _("s"),  _(0.0f), _(0.0f), _(0.0f),
        _(0.0f), _("s"),  _(0.0f), _(0.0f),
        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
    });

    //auto* AORSP = *(*(*(*A * O) * R) * S) * P;
    auto* AORSP = *R * (*O * A);
    //auto* RT = *T->copy() * R->copy();

    std::cout << "Transform: " << AORSP->format(latex) << std::endl;
    //std::cout << "R*T: " << RT->format(latex) << std::endl;

    delete AORSP;

    return 0;
}