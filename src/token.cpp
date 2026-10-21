// File: token.cpp
#include "token.h"


std::string Token::kindName() const
{
    switch(kind)
    {
        case Kind::INT: return "INT";
        case Kind::BOOL: return "BOOL";
        case Kind::IF: return "IF";
        case Kind::ELSE: return "ELSE";
        case Kind::WHILE: return "WHILE";
        case Kind::RETURN: return "RETURN";
        case Kind::TRUE: return "TRUE";
        case Kind::FALSE: return "FALSE";
        case Kind::PRINT: return "PRINT";
        case Kind::READ: return "READ";
        case Kind::IDENT: return "IDENT";
        case Kind::NUM: return "NUM";
        case Kind::PLUS: return "PLUS";
        case Kind::MINUS: return "MINUS";
        case Kind::STAR: return "STAR";
        case Kind::SLASH: return "SLASH";
        case Kind::PERCENT: return "PERCENT";
        case Kind::LT: return "LT";
        case Kind::LE: return "LE";
        case Kind::GT: return "GT";
        case Kind::GE: return "GE";
        case Kind::EQ: return "EQ";
        case Kind::NE: return "NE";
        case Kind::AND: return "AND";
        case Kind::OR: return "OR";
        case Kind::NOT: return "NOT";
        case Kind::ASSIGN: return "ASSIGN";
        case Kind::LPAREN: return "LPAREN";
        case Kind::RPAREN: return "RPAREN";
        case Kind::LBRACE: return "LBRACE";
        case Kind::RBRACE: return "RBRACE";
        case Kind::COMMA: return "COMMA";
        case Kind::SEMI: return "SEMI";
        case Kind::_EOF: return "EOF";
    }
    return "";
}
