//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <exception>

struct InvalidExpression : public std::exception
{
    [[nodiscard]] const char * what () const noexcept override
    {
        return "The expression is not valid.";
    }
};


