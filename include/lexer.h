#pragma once
#include <string>

namespace minique {

enum Token {
    tok_eof = -1,
    tok_set = -2,
    tok_show = -3,
    tok_identifier = -4,
    tok_number = -5,
    tok_string = -6,
    tok_semicolon = -7,
};

class Lexer {
    std::string Source;
    size_t Pos;
    char LastChar;
    
    char getChar();
    
public:
    std::string IdentifierStr;
    double NumVal;
    std::string StringVal;
    
    Lexer(const std::string& source);
    int gettok();
};

} // namespace minique
