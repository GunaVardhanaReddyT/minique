#include "lexer.h"
#include <cctype>

namespace minique {

Lexer::Lexer(const std::string& source) : Source(source), Pos(0), LastChar(' ') {}

char Lexer::getChar() {
    if (Pos >= Source.length())
        return EOF;
    return Source[Pos++];
}

int Lexer::gettok() {
    // Skip whitespace
    while (isspace(LastChar))
        LastChar = getChar();
    
    // Keywords and identifiers
    if (isalpha(LastChar) || LastChar == '_') {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getChar())) || LastChar == '_')
            IdentifierStr += LastChar;
        
        if (IdentifierStr == "set") return tok_set;
        if (IdentifierStr == "show") return tok_show;
        
        return tok_identifier;
    }
    
    // Numbers: 123, 3.14
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getChar();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }
    
    // Strings: "Hello World"
    if (LastChar == '"') {
        LastChar = getChar();
        StringVal = "";
        while (LastChar != '"' && LastChar != EOF) {
            if (LastChar == '\\') {
                LastChar = getChar();
                if (LastChar == 'n') StringVal += '\n';
                else if (LastChar == 't') StringVal += '\t';
                else if (LastChar == '\\') StringVal += '\\';
                else if (LastChar == '"') StringVal += '"';
                else StringVal += LastChar;
            } else {
                StringVal += LastChar;
            }
            LastChar = getChar();
        }
        if (LastChar == '"')
            LastChar = getChar();
        return tok_string;
    }
    
    // Semicolon (statement terminator)
    if (LastChar == ';') {
        LastChar = getChar();
        return tok_semicolon;
    }
    
    // Operators: +, -, *, /, =, (, )
    if (LastChar == '+' || LastChar == '-' || LastChar == '*' || 
        LastChar == '/' || LastChar == '=' || LastChar == '(' || LastChar == ')') {
        int ThisChar = LastChar;
        LastChar = getChar();
        return ThisChar;
    }
    
    if (LastChar == EOF)
        return tok_eof;
    
    // Unknown character - skip it
    int ThisChar = LastChar;
    LastChar = getChar();
    return ThisChar;
}

} // namespace minique
