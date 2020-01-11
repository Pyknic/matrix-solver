#include <sstream>
#include <iostream>
#include <functional>
#include "constant.hpp"
#include "variable.hpp"
#include "matrix.hpp"
#include "helper.hpp"
#include "latex-formatter.hpp"
#include "glm-formatter.hpp"
#include "parser.hpp"

int main(int argc, char **argv) {

    Parser parser {};

    std::stringstream text {
        "x = 5 + 2;\n"
        "y = 3 - x;"
    };

    if (parser.parse(text)) {
        std::cout << "Compilation successful!" << std::endl;
        LatexFormatter latex {};
        std::cout << "y = " << parser.get("y")->format(latex) << std::endl;
    }


//    // The matrix below is a model-to-world matrix
//
//    GlmFormatter toGLM{};
//
//    auto* A = Matrix::square({
//        _("a_x"), _(0.0f),  _(0.0f), _(0.0f),
//        _(0.0f),  _("a_y"), _(0.0f), _(0.0f),
//        _(0.0f),  _(0.0f),  _(1.0f), _(0.0f),
//        _(0.0f),  _(0.0f),  _(0.0f), _(1.0f)
//    });
//
//    auto* O = Matrix::square({
//        _(1.0f), _(0.0f), _(0.0f), -*_("o_x"),
//        _(0.0f), _(1.0f), _(0.0f), *_("o_y")-_(1.0f),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* P = Matrix::square({
//        _(1.0f), _(0.0f), _(0.0f), _("p_x"),
//        _(0.0f), _(1.0f), _(0.0f), _("p_y"),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* R = Matrix::square({
//          _("cosR"), _("sinR"), _(0.0f), _(0.0f),
//        -*_("sinR"), _("cosR"), _(0.0f), _(0.0f),
//            _(0.0f),   _(0.0f), _(1.0f), _(0.0f),
//            _(0.0f),   _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* S = Matrix::square({
//        _("s_x"), _(0.0f), _(0.0f), _(0.0f),
//        _(0.0f), _("s_y"), _(0.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(1.0f), _(0.0f),
//        _(0.0f), _(0.0f), _(0.0f), _(1.0f)
//    });
//
//    auto* M = Matrix::square({
//        _(1),  _(0), _(0), _(0),
//        _(0), _(-1), _(0), _(0),
//        _(0),  _(0), _(1), _(0),
//        _(0),  _(0), _(0), _(1)
//    });
//
//    auto* modelToWorld = *P * (*M * (*S * (*R * (*A * O))));
//
//    std::cout << "mWorldMatrix = " << modelToWorld->format(toGLM) << std::endl;
//
//    delete modelToWorld;

    return 0;
}