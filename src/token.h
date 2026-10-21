// File: token.h
#pragma once
#include <string>


class Token {
public:
    enum class Kind { 
        INT, BOOL,
        IF, ELSE, WHILE, RETURN, TRUE, FALSE, PRINT, READ, 
        IDENT, NUM,
        PLUS, MINUS, STAR, SLASH, PERCENT, LT, LE, GT, GE, EQ, NE, AND, OR, NOT,
        ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMI, _EOF,
    };
    Token(int l, Kind k, std::string v = "") : line(l), kind(k), value(v) {}
    
    std::string kindName() const;

    int line;
    Kind kind;
    std::string value;
private:
};
