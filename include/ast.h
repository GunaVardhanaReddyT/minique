#pragma once
#include <memory>
#include <string>
#include <vector>
#include "llvm/IR/Value.h"

namespace minique {

// Expression base class
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual llvm::Value* codegen() = 0;
};

// Number literal: 42, 3.14
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double Val) : Val(Val) {}
    llvm::Value* codegen() override;
};

// String literal: "Hello"
class StringExprAST : public ExprAST {
    std::string Val;
public:
    StringExprAST(const std::string& Val) : Val(Val) {}
    llvm::Value* codegen() override;
};

// Variable reference: x
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(const std::string& Name) : Name(Name) {}
    llvm::Value* codegen() override;
    const std::string& getName() const { return Name; }
};

// Binary operation: x + y
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    llvm::Value* codegen() override;
};

// Statement base class
class StmtAST {
public:
    virtual ~StmtAST() = default;
    virtual void codegen() = 0;
};

// Assignment: set x = 5
class AssignStmtAST : public StmtAST {
    std::string Name;
    std::unique_ptr<ExprAST> Value;
public:
    AssignStmtAST(const std::string& Name, std::unique_ptr<ExprAST> Value)
        : Name(Name), Value(std::move(Value)) {}
    void codegen() override;
};

// Print: show x
class PrintStmtAST : public StmtAST {
    std::unique_ptr<ExprAST> Value;
public:
    PrintStmtAST(std::unique_ptr<ExprAST> Value) : Value(std::move(Value)) {}
    void codegen() override;
};

} // namespace minique
