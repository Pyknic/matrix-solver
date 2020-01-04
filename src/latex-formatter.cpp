#include "latex-formatter.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "constant.hpp"

LatexFormatter::~LatexFormatter() = default;

std::string LatexFormatter::constant(const Constant *constant) const {
    std::ostringstream ss;
    ss << constant->getValue();
    std::string s(ss.str());
    return s;

    //return std::to_string(constant->getValue());
}

std::string LatexFormatter::unknown(const std::string &unknown) const {
    if (unknown == "alpha"   || unknown == "beta" || unknown == "gamma"
    ||  unknown == "delta"   || unknown == "rho"  || unknown == "sigma"
    ||  unknown == "epsilon" || unknown == "eta"  || unknown == "mu"
    ||  unknown == "tau") { // TODO: This is not a complete list
        return "\\" + unknown;
    }
    return unknown;
}

std::string LatexFormatter::paranthesis(const std::string &middle) const {
    return "\\left(" + middle + "\\right)";
}

std::string LatexFormatter::plus(const std::string &left,
                                 const std::string &right) const {
    return left + " + " + right;
}

std::string LatexFormatter::minus(const std::string &left,
                                  const std::string &right) const {
    return left + " - " + right;
}

std::string LatexFormatter::times(const std::string &left,
                                  const std::string &right) const {
    return left + " " + right;
}

std::string LatexFormatter::divide(const std::string &top,
                                   const std::string &bottom) const {
    return "\\frac{" + top + "}{" + bottom + "}";
}

std::string LatexFormatter::power(const std::string &left,
                                  const std::string &right) const {
    return left + "^{" + right + "}";
}