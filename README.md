# Matrix Solver
Solver for mathematical expressions written in C++.

**Note:** This is a hobby project, so it is not well tested. Feel free to use it at your own risk.

## Source Code
This project is using submodules, so to clone it, do the following:

```shell
git clone --recurse-submodules git@github.com:Pyknic/matrix-solver.git
```

## Usage
The most simple use-case is as a regular calculator:

```shell
echo "answer=1+1;" | cmake-build-debug/matrix_solver_cpp -f answer
```

The code above prints out `2` to the standard output.

### Symbolic Expressions
Another use-case is to simplify symbolic expressions with multiple unknowns.

```shell
echo "answer=5*x+10*y-3*x;" | cmake-build-debug/matrix_solver_cpp -f answer
```

The code above prints out `(10*y+2*x)` to the standard output.

### Matrices
Of course it can also handle matrix multiplications.

```shell
echo "A=[2,0;0,3]; B=[a,b;c,d]; answer=A*B;" | cmake-build-debug/matrix_solver_cpp -f answer 
```

This prints `[2*a,2*b;3*c,3*d]` to the standard output.

### Pretty Printing
The output can be formatted automatically by adding the `--pretty` flag.
```shell
echo "A=[2,0;0,3]; B=[a,b;c,d]; answer=A*B;" | cmake-build-debug/matrix_solver_cpp --pretty
```

This outputs:

```shell
A = [
    2, 0;
    0, 3
];
B = [
    a, b;
    c, d
];
answer = [
    2 * a, 2 * b;
    3 * c, 3 * d
];
```

### Formatting
**In Progress**

## License
MIT License

Copyright (c) 2020 Emil Forslund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.