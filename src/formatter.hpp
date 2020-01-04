//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <string>
#include "constant.hpp"

class Formatter {
public:
    virtual ~Formatter() = 0;

    virtual std::stringstream format(const Constant* constant) const = 0;

    virtual std::stringstream unknown(const std::string& unknown) const = 0;

    virtual std::stringstream paranthesis(const std::stringstream& middle) const = 0;

    virtual std::stringstream plus(const std::stringstream& left, const std::stringstream& right) const = 0;

    virtual std::stringstream minus(const std::stringstream& left, const std::stringstream& right) const = 0;

    virtual std::stringstream times(const std::stringstream& left, const std::stringstream& right) const = 0;

    virtual std::stringstream divide(const std::stringstream& top, const std::stringstream& left) const = 0;

    virtual std::stringstream power(const std::stringstream& left, const std::stringstream& right) const = 0;
};


