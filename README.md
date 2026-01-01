# MINIQUE



## Overview

Minique is an educational programming language designed to demystify how programs actually work. By implementing a simple yet complete language with LLVM, you can trace every step from source code to machine execution.

**Core Philosophy:** Understanding happens when you build it yourself.

## Syntax

### Variables

```minique
set x = 42;
set name = "John";
set result = x + 10;
```
Operators
```
Addition: +
```
```
Subtraction: -
```
```
Multiplication: *
```
```
Division: /
```
```
Parentheses: ()
```
Output
```
show "Hello";
show x;
show x + y;
```
Example Program
```
set a = 10;
set b = 20;
set sum = a + b;
show "Result:";
show sum;
```

Output:
```
Result:
30
```

Build Requirements
LLVM 14+ : Core compilation infrastructure

CMake 3.13+ : Build system

C++17 compiler : Language implementation

Installation
# Clone the repository
git clone https://github.com/GunaVardhanaReddyT/minique.git

cd minique

# Build
mkdir build
cd build
cmake .. -DLLVM_DIR=/usr/lib/llvm-14/lib/cmake/llvm
make

Usage:

# Run a Minique program
./minique program.mq

Current Features (v1.0)
 Variable assignment
 Arithmetic expressions
 String and number literals
 Print statements
 LLVM-powered execution

| Version | Features                              | Status         |
| ------- | ------------------------------------- | -------------- |
| v1.0    | Variables, arithmetic, output         |  Released     |
| v2.0    | Conditionals (if/else), loops (while) |  In Progress |
| v3.0    | Arrays and data structures            |  Planned     |

License
MIT License

"The best way to understand compilers is to write one."




