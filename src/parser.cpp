#include "parser.hpp"

//
// Copyright (c) 2020 Emil Forslund. All rights reserved.
//

#include <vector>
#include <sstream>
#include "symbol.hpp"
#include "variable.hpp"
#include "matrix.hpp"
#include "constant.hpp"
#include "optimizer.hpp"

Parser::Parser() : mDefines{}, mLine{0}, mCol{0}, mDone{false} {}

Parser::~Parser() {
    for (auto& define : mDefines) {
        delete define.second;
    }
}

std::string Parser::format(const Formatter& formatter) const {
    std::ostringstream ss;

    for (auto& define : mDefines) {
        ss << formatter.assign(define.first, define.second->format(formatter));
    }

    return ss.str();
}

bool Parser::parse(std::istream &input) {
    while (!mDone) {
        char c;
        if (nextNonWhitespace(input, c)) {
            char nameTerminated;
            auto name = expectName(input, nameTerminated, c);

            if (nameTerminated != '=')
                throw unexpectedCharacter(nameTerminated);

            if (mDefines.find(name) != mDefines.end()) {
                throw parseError("Variable '" + name + "' already defined.");
            }

            auto* symbol = expectSymbolsUntil(input, ';');
            mDefines[name] = symbol;
        } else {
            mDone = true;
        }
    }

    for (auto& pair : mDefines) {
        for (auto& inner : mDefines) {
            if (inner.first == pair.first) continue;
            inner.second = inner.second->replace(
                [pair](const Symbol* symbol) -> bool {
                    if (auto* variable = dynamic_cast<const Variable*>(symbol)) {
                        return variable->getName() == pair.first;
                    } else return false;
                }, [pair](Symbol* symbol) -> Symbol* {
                    delete symbol;
                    return pair.second->copy();
                }
            );
        }
    }

    Optimizer optimizer {};
    for (auto& pair : mDefines) {
        pair.second = optimizer.optimize(pair.second);
    }

    return true; // Parsing was successful
}

Symbol *Parser::get(const std::string &key) const {
    auto define = mDefines.find(key);
    if (define == mDefines.end()) {
        throw std::invalid_argument("No symbol with key '" + key + "'.");
    }
    return define->second;
}

bool Parser::nextChar(std::istream &input, char &c) {
    int i = input.get();
    switch (i) {
        case EOF: return false;
        case '\n': {
            mLine++; // TODO: This should not be incremented before the character is parsed.
            mCol = 0;
            break;
        }
        default: mCol++;
    }

    c = (char) i;
    return true;
}

#define CASE_WHITESPACE \
    case ' ': case '\n':

#define CASE_0_TO_9 \
    case '0': case '1': case '2': case '3': case '4': \
    case '5': case '6': case '7': case '8': case '9':

#define CASE_ALPHABETIC \
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': \
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': \
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': \
    case 'v': case 'w': case 'x': case 'y': case 'z': \
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': \
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': \
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': \
    case 'V': case 'W': case 'X': case 'Y': case 'Z':

bool Parser::nextNonWhitespace(std::istream &input, char &c) {
    while (nextChar(input, c)) {
        switch (c) {
            CASE_WHITESPACE continue;
            default: return true;
        }
    }
    return false;
}

std::string Parser::expectName(std::istream &input, char &terminatedBy, char firstLetter) {
    std::stringstream name{};
    name.put(firstLetter);

    char c;
    while (nextChar(input, c)) {
        switch (c) {
            case '_':
            CASE_ALPHABETIC name.put(c); continue;
            CASE_WHITESPACE {
                if (!nextNonWhitespace(input, terminatedBy))
                    throw unexpectedEndOfFile();
                break;
            }
            default: terminatedBy = c;
        }
        return name.str();
    }

    throw unexpectedEndOfFile();
}

Symbol *Parser::expectOneSymbol(std::istream &input, char &terminatedBy) {
    bool negative = false;

    char c;
    while (nextNonWhitespace(input, c)) {
        switch (c) {
            case '-': {
                if (negative) throw unexpectedCharacter(c);
                negative = true;
                continue;
            }
            case '[': {
                Symbol* symbol = expectMatrix(input);
                if (nextNonWhitespace(input, terminatedBy))
                    return negative ? symbol->negate() : symbol;
                throw unexpectedEndOfFile();
            }
            case '(': {
                Symbol* symbol = expectSymbolsUntil(input, ')');
                if (nextNonWhitespace(input, terminatedBy))
                    return negative ? symbol->negate() : symbol;
                throw unexpectedEndOfFile();
            }
            case '.': {
                const float decimal = expectDecimalPart(input, terminatedBy);
                Symbol* symbol = new Constant{decimal};
                return negative ? symbol->negate() : symbol;
            }
            CASE_0_TO_9 {
                Symbol* symbol = expectConstant(input, terminatedBy, c);
                return negative ? symbol->negate() : symbol;
            }
            CASE_ALPHABETIC {
                Symbol* symbol = expectVariable(input, terminatedBy, c);
                return negative ? symbol->negate() : symbol;
            }
            default: throw unexpectedCharacter(c);
        }
    }

    throw unexpectedEndOfFile();
}

Symbol *Parser::expectSymbolsUntil(std::istream &input, char termination) {
    char _;
    return expectSymbolsUntilAny(input,
        [&termination](char c) -> bool {
            return termination == c;
        }, _);
}

Symbol *Parser::expectSymbolsUntilAny(std::istream &input,
                                      const std::function<bool(char)>& predicate,
                                      char &terminatedBy) {
    auto* symbol = expectOneSymbol(input, terminatedBy);
    while (!predicate(terminatedBy)) {
        switch (terminatedBy) { // TODO: Add more operators, like power
            case '+': {
                Symbol* right = expectSymbolsUntilAny(input, predicate, terminatedBy);
                symbol = *symbol + right;
                return symbol;
            }
            case '-': {
                Symbol* right = expectSymbolsUntilAny(input, predicate, terminatedBy);
                symbol = *symbol - right;
                return symbol;
            }
            case '*': {
                Symbol* right = expectOneSymbol(input, terminatedBy);
                symbol = *symbol * right;
                continue;
            }
            case '(': {
                Symbol* right = expectSymbolsUntil(input, ')');
                symbol = *symbol * right;
                break;
            }
            case '/': {
                Symbol* right = expectOneSymbol(input, terminatedBy);
                symbol = *symbol / right;
                continue;
            }
            case '\'': {
                if (auto* leftMatrix = dynamic_cast<Matrix*>(symbol)) {
                    symbol = leftMatrix->transpose();
                    break;
                } else {
                    throw unexpectedCharacter('\'');
                }
            }
            default: throw unexpectedCharacter(terminatedBy);
        }
        if (!nextNonWhitespace(input, terminatedBy))
            throw unexpectedEndOfFile();
    }

    return symbol;
}

Matrix *Parser::expectMatrix(std::istream &input) {
    std::vector<Symbol*> row {};

    char elementEndedWith;
    do {
        Symbol* element = expectSymbolsUntilAny(input, [](char c) -> bool {
            switch (c) {
                case ',': case ';': case ']': return true;
                default: return false;
            }
        }, elementEndedWith);
        row.push_back(element);
    } while (elementEndedWith == ',');

    switch (elementEndedWith) {
        case ']': {
            Matrix* matrix = Matrix::zero(1, row.size());
            for (int i = 0; i < row.size(); i++) {
                matrix->set(0, i, row[i]);
            }
            return matrix;
        }
        case ';': {
            std::vector<std::vector<Symbol*>> rows{};
            rows.push_back(row);

            while (elementEndedWith == ';') {
                rows.push_back(expectMatrixRow(input, row.size(), elementEndedWith));
            }

            if (elementEndedWith != ']') {
                throw unexpectedCharacter(elementEndedWith);
            }

            Matrix* matrix = Matrix::zero(rows.size(), row.size());
            for (int i = 0; i < rows.size(); i++) {
                for (int j = 0; j < row.size(); j++) {
                    matrix->set(i, j, rows[i][j]);
                }
            }
            return matrix;
        }
        default: throw unexpectedCharacter(elementEndedWith);
    }

    throw unexpectedEndOfFile();
}

std::vector<Symbol*> Parser::expectMatrixRow(std::istream &input, int columns, char &terminatedBy) {
    std::vector<Symbol*> row {};

    char elementEndedWith;
    do {
        Symbol* element = expectSymbolsUntilAny(input, [](char c) -> bool {
            switch (c) {
                case ',': case ';': case ']': return true;
                default: return false;
            }
        }, elementEndedWith);
        row.push_back(element);
    } while (elementEndedWith == ',');

    terminatedBy = elementEndedWith;
    if (row.size() != columns) {
        throw parseError("Expected row to have " + std::to_string(columns) +
            " columns but it only had " + std::to_string(row.size()));
    }

    return row;
}

Variable *Parser::expectVariable(std::istream &input, char &terminatedBy, char firstLetter) {
    std::string name = expectName(input, terminatedBy, firstLetter);
    return new Variable{name};
}

Constant *Parser::expectConstant(std::istream &input, char &terminatedBy, char zeroToNineOrMinus) {
    std::stringstream number{};
    number.put(zeroToNineOrMinus);

    char c;
    while (nextChar(input, c)) {
        switch (c) {
            case '_': continue;
            case '.': {
                const float decimal = expectDecimalPart(input, terminatedBy);
                return new Constant{std::stoi(number.str()) + decimal};
            }
            CASE_0_TO_9 {
                number.put(c);
                continue;
            }
            CASE_WHITESPACE {
                if (!nextNonWhitespace(input, terminatedBy))
                    throw unexpectedEndOfFile();
                break;
            }
            default: {
                terminatedBy = c;
                break;
            }
        }
        return new Constant{(float) std::stoi(number.str())};
    }

    throw unexpectedEndOfFile();
}

float Parser::expectDecimalPart(std::istream &input, char &terminatedBy) {
    std::stringstream number{};
    number << "0.";

    char c;
    while (nextChar(input, c)) {
        switch (c) {
            case '_': continue;
            CASE_0_TO_9 {
                number.put(c);
                continue;
            }
            CASE_WHITESPACE {
                if (nextNonWhitespace(input, terminatedBy))
                    throw unexpectedEndOfFile();
                break;
            }
            default: {
                terminatedBy = c;
                break;
            }
        }
        std::string result = number.str();
        return std::stof(result);
    }

    throw unexpectedEndOfFile();
}

std::invalid_argument Parser::unexpectedEndOfFile() const {
    return parseError("Unexpected end of file.");
}

std::invalid_argument Parser::unexpectedCharacter(char c) const {
    return parseError("Unexpected character '" + std::to_string(c) + "'.");
}

std::invalid_argument Parser::parseError(const std::string &message) const {
    return std::invalid_argument(
        "Parse error at " +
        std::to_string(mLine) + ":" +
        std::to_string(mCol) + ": " + message);
}
