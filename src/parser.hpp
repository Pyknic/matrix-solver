#pragma once

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <istream>
#include <string>
#include <functional>
#include <vector>
#include <map>

class Symbol;
class Matrix;
class Variable;
class Constant;

class Parser {
public:
    explicit Parser();

    ~Parser();

    bool parse(std::istream& input);

    Symbol* get(const std::string& key) const;

private:
    std::map<std::string, Symbol*> mDefines;
    uint32_t mLine, mCol; bool mDone;

    bool nextChar(std::istream& input, char& c);

    bool nextNonWhitespace(std::istream& input, char& c);

    std::string expectName(std::istream& input, char& terminatedBy);

    std::string expectName(std::istream &input, char &terminatedBy, char firstLetter);

    Symbol* expectOneSymbol(std::istream &input, char &terminatedBy);

    Symbol* expectSymbolsUntil(std::istream& input, char termination);

    Symbol* expectSymbolsUntilAny(std::istream& input, const std::function<bool(char)>& predicate, char &terminatedBy);

    Matrix* expectMatrix(std::istream& input);

    std::vector<Symbol*> expectMatrixRow(std::istream& input, int columns, char &terminatedBy);

    Variable* expectVariable(std::istream &input, char &terminatedBy, char firstLetter);

    Constant* expectConstant(std::istream &input, char &terminatedBy, char zeroToNine);

    float expectDecimalPart(std::istream& input, char& terminatedBy);

    std::invalid_argument unexpectedEndOfFile() const;

    std::invalid_argument unexpectedCharacter(char c) const;

    std::invalid_argument parseError(const std::string& message) const;
};
