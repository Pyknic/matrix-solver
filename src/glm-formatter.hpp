#pragma once

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "formatter.hpp"

class GlmFormatter : public Formatter {
public:
    std::string constant(const Constant *constant) const override;

    std::string matrix(int rows, int cols,
                       const std::vector<std::string> &elements) const override;

    std::string unknown(const std::string &unknown) const override;

    std::string paranthesis(const std::string &middle) const override;

    std::string
    plus(const std::string &left, const std::string &right) const override;

    std::string
    minus(const std::string &left, const std::string &right) const override;

    std::string
    times(const std::string &left, const std::string &right) const override;

    std::string
    divide(const std::string &top, const std::string &bottom) const override;

    std::string
    power(const std::string &left, const std::string &right) const override;

    std::string negate(const std::string &unknown) const override;

    std::string assign(const std::string& name, const std::string& value) const override;
};
