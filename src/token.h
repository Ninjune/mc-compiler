// File: token.h
#pragma once
#include <string>

// also terminals
enum TokenKind { 
    INT, BOOL,
    IF, ELSE, WHILE, RETURN, TRUE, FALSE, PRINT, READ, 
    IDENT, NUM,
    PLUS, MINUS, STAR, SLASH, PERCENT, LT, LE, GT, GE, EQ, NE, AND, OR, NOT,
    ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMI, _EOF,
};

class Token {
public:
    
    Token(int l, TokenKind k, std::string v = "") : line(l), kind(k), value(v) {}
    
    std::string kindName() const;

    int line;
    TokenKind kind;
    std::string value;
private:
};
