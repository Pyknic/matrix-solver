#include "glm-formatter.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "constant.hpp"
#include <sstream>
#include <algorithm>
#include <string>

std::string GlmFormatter::constant(const Constant *constant) const {
    std::ostringstream ss;
    ss << constant->getValue();
    std::string s(ss.str());

    if (constant->getValue() < 0.0f) {
        return paranthesis(s);
    } else return s;
}

std::string GlmFormatter::matrix(int rows, int cols,
                                 const std::vector<std::string> &elements) const {
    std::stringstream ss;
    ss << "{\n";

    for (int j = 0; j < cols; j++) {
        ss << "    ";

        for (int i = 0; i < rows; i++) {
            if (i > 0) ss << ", ";
            ss << elements[i * cols + j];
        }

        ss << ",\n";
    }

    ss << "};";
    return ss.str();
}

std::string GlmFormatter::unknown(const std::string &unknown) const {
    std::string copy = unknown;
    std::replace(copy.begin(), copy.end(), '_', '.');
    return copy;
}

std::string GlmFormatter::paranthesis(const std::string &middle) const {
    return "(" + middle + ")";
}

std::string
GlmFormatter::plus(const std::string &left, const std::string &right) const {
    return left + " + " + right;
}

std::string
GlmFormatter::minus(const std::string &left, const std::string &right) const {
    return left + " - " + right;
}

std::string
GlmFormatter::times(const std::string &left, const std::string &right) const {
    return left + " * " + right;
}

std::string
GlmFormatter::divide(const std::string &top, const std::string &bottom) const {
    return top + " / " + bottom;
}

std::string
GlmFormatter::power(const std::string &left, const std::string &right) const {
    if ("2" == right) {
        return left + " * " + left;
    } else {
        return "pow(" + left + ", " + right + ")";
    }
}

std::string GlmFormatter::negate(const std::string &unknown) const {
    return "-" + unknown;
}