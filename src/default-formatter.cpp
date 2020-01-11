#include "default-formatter.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "constant.hpp"
#include <sstream>

DefaultFormatter::DefaultFormatter(bool pretty) :
    pretty{pretty}, tab{"    "}, nl{"\n"} {}

DefaultFormatter::~DefaultFormatter() = default;

std::string DefaultFormatter::constant(const Constant* constant) const {
    std::ostringstream ss;
    ss << constant->getValue();
    std::string s(ss.str());

    if (constant->getValue() < 0.0f) {
        return paranthesis(s);
    } else return s;
}

std::string DefaultFormatter::matrix(int rows, int cols,
                                     const std::vector<std::string>& elements) const {
    std::ostringstream ss;
    ss << "[";
    for (int i = 0; i < rows; i++) {
        if (pretty) ss << nl << tab;
        for (int j = 0; j < cols; j++) {
            if (j > 0) ss << prettyComma();
            ss << elements[i * cols + j];
        }
        if (i < rows - 1) ss << ";";
    }
    if (pretty) ss << nl;
    ss << "]";

    return ss.str();
}

std::string DefaultFormatter::unknown(const std::string& unknown) const {
//    std::string copy = unknown;
//    std::replace(copy.begin(), copy.end(), '_', '.');
    return unknown;
}

std::string DefaultFormatter::paranthesis(const std::string& middle) const {
    return pretty ? ("( " + middle + " )") : ("(" + middle + ")");
}

std::string DefaultFormatter::plus(const std::string& left,
                                   const std::string& right) const {
    return pretty ? (left + " + " + right) : (left + "+" + right);
}

std::string DefaultFormatter::minus(const std::string& left,
                                    const std::string& right) const {
    return pretty ? (left + " - " + right) : (left + "-" + right);
}

std::string DefaultFormatter::times(const std::string& left,
                                    const std::string& right) const {
    return pretty ? (left + " * " + right) : (left + "*" + right);
}

std::string DefaultFormatter::divide(const std::string& top,
                                     const std::string& bottom) const {
    return pretty ? (top + " / " + bottom) : (top + "/" + bottom);
}

std::string DefaultFormatter::power(const std::string& left,
                                    const std::string& right) const {
    return left + "^" + right;
}

std::string DefaultFormatter::negate(const std::string& unknown) const {
    return "-" + unknown;
}

std::string DefaultFormatter::assign(const std::string& name,
                                     const std::string& value) const {
    return pretty
        ? (name + " = " + value + ";" + nl)
        : (name + "=" + value + ";");
}

const std::string DefaultFormatter::prettyComma() const {
    return pretty ? ", " : ",";
}
