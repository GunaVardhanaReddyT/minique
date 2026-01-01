███╗   ███╗██╗███╗   ██╗██╗ ██████╗ ██╗   ██╗███████╗
████╗ ████║██║████╗  ██║██║██╔═══██╗██║   ██║██╔════╝
██╔████╔██║██║██╔██╗ ██║██║██║   ██║██║   ██║█████╗  
██║╚██╔╝██║██║██║╚██╗██║██║██║▄▄ ██║██║   ██║██╔══╝  
██║ ╚═╝ ██║██║██║ ╚████║██║╚██████╔╝╚██████╔╝███████╗
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝ ╚══▀▀═╝  ╚═════╝ ╚══════╝

A human-readable programming language

---------------------------------------------------------------------------------

Minique
This programming language is created by me to actually understand the bts (behind the scenes) of what the program actually go through.

Syntax
Variables
text
set x = 42;
set name = "John";
set result = x + 10;
Operators
Addition: +

Subtraction: -

Multiplication: *

Division: /

Parentheses: ()

Output
text
show "Hello";
show x;
show x + y;
Example Program
text
set a = 10;
set b = 20;
set sum = a + b;
show "Result:";
show sum;
Build
Requirements
LLVM 14+

CMake 3.13+

C++17 compiler

Commands
bash
mkdir build
cd build
cmake .. -DLLVM_DIR=/usr/lib/llvm-14/lib/cmake/llvm
make
Usage
bash
./minique program.mq
Implementation
The compiler consists of four main components:

Lexer - tokenizes source code

Parser - builds abstract syntax tree

Code generator - converts AST to LLVM IR

JIT execution engine - compiles and runs code

Current Features (v1.0)
Variable assignment

Arithmetic expressions

String and number literals

Print statements

Planned Features
v2.0: Conditionals and loops

v3.0: Arrays and data structures

License
MIT
