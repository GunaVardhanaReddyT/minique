#include "parser.h"
#include <iostream>

namespace minique {

std::unique_ptr<ExprAST> Parser::ParsePrimary() {
    // Number
    if (CurTok == tok_number) {
        auto Result = std::make_unique<NumberExprAST>(Lex.NumVal);
        getNextToken();
        return Result;
    }
    
    // String
    if (CurTok == tok_string) {
        auto Result = std::make_unique<StringExprAST>(Lex.StringVal);
        getNextToken();
        return Result;
    }
    
    // Variable
    if (CurTok == tok_identifier) {
        std::string IdName = Lex.IdentifierStr;
        getNextToken();
        return std::make_unique<VariableExprAST>(IdName);
    }
    
    // Parentheses
    if (CurTok == '(') {
        getNextToken();
        auto Expr = ParseExpression();
        if (CurTok == ')')
            getNextToken();
        return Expr;
    }
    
    return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseMultiplicative() {
    auto LHS = ParsePrimary();
    if (!LHS) return nullptr;
    
    // Handle * and /
    while (CurTok == '*' || CurTok == '/') {
        char Op = CurTok;
        getNextToken();
        auto RHS = ParsePrimary();
        if (!RHS) return nullptr;
        LHS = std::make_unique<BinaryExprAST>(Op, std::move(LHS), std::move(RHS));
    }
    
    return LHS;
}

std::unique_ptr<ExprAST> Parser::ParseAdditive() {
    auto LHS = ParseMultiplicative();
    if (!LHS) return nullptr;
    
    // Handle + and -
    while (CurTok == '+' || CurTok == '-') {
        char Op = CurTok;
        getNextToken();
        auto RHS = ParseMultiplicative();
        if (!RHS) return nullptr;
        LHS = std::make_unique<BinaryExprAST>(Op, std::move(LHS), std::move(RHS));
    }
    
    return LHS;
}

std::unique_ptr<ExprAST> Parser::ParseExpression() {
    return ParseAdditive();
}

std::unique_ptr<StmtAST> Parser::ParseStatement() {
    // Assignment: set x = 5;
    if (CurTok == tok_set) {
        getNextToken();
        if (CurTok != tok_identifier) {
            std::cerr << "Expected variable name after 'set'\n";
            return nullptr;
        }
        std::string VarName = Lex.IdentifierStr;
        getNextToken();
        
        if (CurTok != '=') {
            std::cerr << "Expected '=' after variable name\n";
            return nullptr;
        }
        getNextToken();
        
        auto Expr = ParseExpression();
        if (!Expr) return nullptr;
        
        // REQUIRE semicolon
        if (CurTok != tok_semicolon) {
            std::cerr << "Error: Expected ';' at end of statement\n";
            return nullptr;
        }
        getNextToken();
        
        return std::make_unique<AssignStmtAST>(VarName, std::move(Expr));
    }
    
    // Print: show x;
    if (CurTok == tok_show) {
        getNextToken();
        auto Expr = ParseExpression();
        if (!Expr) return nullptr;
        
        // REQUIRE semicolon
        if (CurTok != tok_semicolon) {
            std::cerr << "Error: Expected ';' at end of statement\n";
            return nullptr;
        }
        getNextToken();
        
        return std::make_unique<PrintStmtAST>(std::move(Expr));
    }
    
    return nullptr;
}


std::vector<std::unique_ptr<StmtAST>> Parser::Parse() {
    std::vector<std::unique_ptr<StmtAST>> Stmts;
    getNextToken(); // Prime the lexer
    
    while (CurTok != tok_eof) {
        auto Stmt = ParseStatement();
        if (Stmt) {
            Stmts.push_back(std::move(Stmt));
        } else {
            // Skip unknown tokens
            getNextToken();
        }
    }
    
    return Stmts;
}

} // namespace minique
