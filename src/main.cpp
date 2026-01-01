#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "ast.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace minique;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Minique v1.0.0 - A Human-Readable Programming Language\n";
        std::cerr << "Usage: minique <file.mq>\n";
        return 1;
    }
    
    // Read source file
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Cannot open file '" << argv[1] << "'\n";
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    
    // Parse
    Lexer lexer(source);
    Parser parser(lexer);
    
    InitializeModule();
    
    // Create main function
    llvm::FunctionType* MainType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*TheContext), false);
    llvm::Function* MainFunc = llvm::Function::Create(
        MainType, llvm::Function::ExternalLinkage, "main", TheModule.get());
    
    llvm::BasicBlock* Entry = llvm::BasicBlock::Create(*TheContext, "entry", MainFunc);
    Builder->SetInsertPoint(Entry);
    
    // Generate code
    auto Stmts = parser.Parse();
    for (auto& Stmt : Stmts) {
        Stmt->codegen();
    }
    
    // Return 0
    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    
    // Verify
    std::string ErrorStr;
    llvm::raw_string_ostream ErrorStream(ErrorStr);
    if (llvm::verifyModule(*TheModule, &ErrorStream)) {
        std::cerr << "Module verification failed:\n" << ErrorStr << "\n";
        return 1;
    }
    
    // Execute via JIT
    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(TheModule))
        .setErrorStr(&ErrorStr)
        .create();
    
    if (!EE) {
        std::cerr << "Failed to create ExecutionEngine: " << ErrorStr << "\n";
        return 1;
    }
    
    EE->finalizeObject();
    std::vector<llvm::GenericValue> noargs;
    EE->runFunction(MainFunc, noargs);
    
    delete EE;
    return 0;
}
