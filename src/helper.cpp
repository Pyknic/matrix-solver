#include "helper.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include "constant.hpp"
#include "variable.hpp"

Symbol* _(float v) {
    return new Constant{v};
}

Symbol* _(const std::string& s) {
    return new Variable{s};
}