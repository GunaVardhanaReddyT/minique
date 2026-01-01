#pragma once
#include "lexer.h"
#include "ast.h"
#include <memory>
#include <vector>

namespace minique {

class Parser {
    Lexer& Lex;
    int CurTok;
    
    int getNextToken() { return CurTok = Lex.gettok(); }
    
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseMultiplicative();
    std::unique_ptr<ExprAST> ParseAdditive();
    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<StmtAST> ParseStatement();
    
public:
    Parser(Lexer& lex) : Lex(lex), CurTok(0) {}
    std::vector<std::unique_ptr<StmtAST>> Parse();
};

} // namespace minique
