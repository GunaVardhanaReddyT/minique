#include "codegen.h"
#include "ast.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Constants.h"
#include <iostream>

namespace minique {

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value*> NamedValues;

void InitializeModule() {
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("minique_v1", *TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

llvm::Value* LogErrorV(const char* Str) {
    std::cerr << "CodeGen Error: " << Str << std::endl;
    return nullptr;
}

// Number literal
llvm::Value* NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}

// String literal
llvm::Value* StringExprAST::codegen() {
    return Builder->CreateGlobalStringPtr(Val);
}

// Variable reference
llvm::Value* VariableExprAST::codegen() {
    llvm::Value* V = NamedValues[Name];
    if (!V) {
        return LogErrorV(("Unknown variable: " + Name).c_str());
    }
    return V;
}

// Binary operation
llvm::Value* BinaryExprAST::codegen() {
    llvm::Value* L = LHS->codegen();
    llvm::Value* R = RHS->codegen();
    if (!L || !R) return nullptr;
    
    switch (Op) {
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Builder->CreateFMul(L, R, "multmp");
        case '/':
            return Builder->CreateFDiv(L, R, "divtmp");
        default:
            return LogErrorV("Invalid binary operator");
    }
}

// Assignment statement
void AssignStmtAST::codegen() {
    llvm::Value* Val = Value->codegen();
    if (!Val) return;
    NamedValues[Name] = Val;
}

// Print statement
void PrintStmtAST::codegen() {
    llvm::Value* Val = Value->codegen();
    if (!Val) return;
    
    // Declare printf
    std::vector<llvm::Type*> PrintfArgs;
    PrintfArgs.push_back(llvm::Type::getInt8PtrTy(*TheContext));
    llvm::FunctionType* PrintfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*TheContext), PrintfArgs, true);
    llvm::FunctionCallee PrintfFunc = 
        TheModule->getOrInsertFunction("printf", PrintfType);
    
    // Check if it's a string or number
    if (Val->getType()->isPointerTy()) {
        // It's a string
        llvm::Value* FormatStr = Builder->CreateGlobalStringPtr("%s\n");
        Builder->CreateCall(PrintfFunc, {FormatStr, Val});
    } else {
        // It's a number
        llvm::Value* FormatStr = Builder->CreateGlobalStringPtr("%g\n");
        Builder->CreateCall(PrintfFunc, {FormatStr, Val});
    }
}

} // namespace minique
