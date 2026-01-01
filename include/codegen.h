#pragma once
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <memory>

namespace minique {

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value*> NamedValues;

void InitializeModule();
llvm::Value* LogErrorV(const char* Str);

} // namespace minique
