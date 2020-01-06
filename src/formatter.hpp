//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

class Constant;

class Formatter {
public:
    virtual ~Formatter() = default;

    virtual std::string constant(const Constant *constant) const = 0;

    virtual std::string matrix(int rows, int cols, const std::vector<std::string>& elements) const = 0;

    virtual std::string unknown(const std::string &unknown) const = 0;

    virtual std::string paranthesis(const std::string &middle) const = 0;

    virtual std::string plus(const std::string &left, const std::string &right) const = 0;

    virtual std::string minus(const std::string &left, const std::string &right) const = 0;

    virtual std::string times(const std::string &left, const std::string &right) const = 0;

    virtual std::string divide(const std::string &top, const std::string &bottom) const = 0;

    virtual std::string power(const std::string &left, const std::string &right) const = 0;

    virtual std::string negate(const std::string &unknown) const = 0;
};


