// File: token.h
#pragma once
#include <string>
#include <utility>

// also terminals
// clang-format off
enum TokenKind { 
    INT, BOOL,
    IF, ELSE, WHILE, RETURN, TRUE, FALSE, PRINT, READ, 
    IDENT, NUM,
    PLUS, MINUS, STAR, SLASH, PERCENT, LT, LE, GT, GE, EQ, NE, AND, OR, NOT,
    ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMI, 
    EPSILON, _EOF, 
    // note: epsilon will never be lexed, only used for the parser as
    // these are the same as terminals
};
// clang-format on
std::string kindName(TokenKind k);

class Token
{
public:
    Token(int l, TokenKind k, std::string v = "")
        : line(l), kind(k), value(std::move(v))
    {}

    [[nodiscard]] std::string kindName() const;

    int line;
    TokenKind kind;
    std::string value;
private:
};
