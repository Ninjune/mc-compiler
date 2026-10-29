// File: token.cpp
#include "token.h"


std::string kindName(TokenKind kind)
{
    switch(kind)
    {
        case TokenKind::INT: return "INT";
        case TokenKind::BOOL: return "BOOL";
        case TokenKind::IF: return "IF";
        case TokenKind::ELSE: return "ELSE";
        case TokenKind::WHILE: return "WHILE";
        case TokenKind::RETURN: return "RETURN";
        case TokenKind::TRUE: return "TRUE";
        case TokenKind::FALSE: return "FALSE";
        case TokenKind::PRINT: return "PRINT";
        case TokenKind::READ: return "READ";
        case TokenKind::IDENT: return "IDENT";
        case TokenKind::NUM: return "NUM";
        case TokenKind::PLUS: return "PLUS";
        case TokenKind::MINUS: return "MINUS";
        case TokenKind::STAR: return "STAR";
        case TokenKind::SLASH: return "SLASH";
        case TokenKind::PERCENT: return "PERCENT";
        case TokenKind::LT: return "LT";
        case TokenKind::LE: return "LE";
        case TokenKind::GT: return "GT";
        case TokenKind::GE: return "GE";
        case TokenKind::EQ: return "EQ";
        case TokenKind::NE: return "NE";
        case TokenKind::AND: return "AND";
        case TokenKind::OR: return "OR";
        case TokenKind::NOT: return "NOT";
        case TokenKind::ASSIGN: return "ASSIGN";
        case TokenKind::LPAREN: return "LPAREN";
        case TokenKind::RPAREN: return "RPAREN";
        case TokenKind::LBRACE: return "LBRACE";
        case TokenKind::RBRACE: return "RBRACE";
        case TokenKind::COMMA: return "COMMA";
        case TokenKind::SEMI: return "SEMI";
        case TokenKind::EPSILON: return "ε";
        case TokenKind::_EOF: return "EOF";
    }
    return "";
}


std::string Token::kindName() const
{
    return ::kindName(kind);
}
