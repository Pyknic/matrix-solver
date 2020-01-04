//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#pragma once

#include <set>
#include <string>
#include <map>
#include "formatter.hpp"

class Symbol {
public:
    typedef float value_t;

    virtual ~Symbol() = default;

    virtual Symbol* copy() const = 0;

    virtual Symbol* negate() = 0;

    virtual Symbol* operator+(Symbol* other) = 0;

    virtual Symbol* operator-(Symbol* other) = 0;

    virtual Symbol* operator*(Symbol* other) = 0;

    virtual Symbol* operator/(Symbol* other) = 0;

    virtual Symbol* operator+(value_t other);

    virtual Symbol* operator-(value_t other);

    virtual Symbol* operator*(value_t other);

    virtual Symbol* operator/(value_t other);

    virtual std::string format(const Formatter& formatter) const = 0;

    virtual bool isConstant() const = 0;

    virtual int getColumns() const = 0;

    virtual int getRows() const = 0;

    virtual std::set<std::string> findUndefined() = 0;

protected:
    void assertSameDimensions(const Symbol* other) const;

    Symbol* assertNotNull(Symbol* other) const;
};


