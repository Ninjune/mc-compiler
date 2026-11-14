// File: symbol.h
#pragma once
#include "token.h"
#include <unordered_map>
#include <variant>
#include <vector>

enum NonTerminal
{
    S,
    Program,
    FunDecls,
    FunDecl,
    ParamsOpt,
    Params,
    ParamsTail,
    Param,
    Type,
    Block,
    Items,
    Item,
    Decl,
    Stmt,
    StmtRest,
    ElsePart,
    ArgsOpt,
    Args,
    ArgsTail,
    Exp,
    ExpTail,
    AndExp,
    AndTail,
    EqExp,
    EqTail,
    RelExp,
    RelTail,
    AddExp,
    AddTail,
    MulExp,
    MulTail,
    Unary,
    Primary,
    Call,
    NTermSize
};

typedef TokenKind Terminal;
typedef std::variant<NonTerminal, Terminal> Symbol;

namespace {
std::string NTToStr(NonTerminal nt)
{
    switch(nt)
    {
        case S: return "S";
        case Program: return "Program";
        case FunDecls: return "FunDecls";
        case FunDecl: return "FunDecl";
        case ParamsOpt: return "ParamsOpt";
        case Params: return "Params";
        case ParamsTail: return "ParamsTail";
        case Param: return "Param";
        case Type: return "Type";
        case Block: return "Block";
        case Items: return "Items";
        case Item: return "Item";
        case Decl: return "Decl";
        case Stmt: return "Stmt";
        case StmtRest: return "StmtRest";
        case ElsePart: return "ElsePart";
        case ArgsOpt: return "ArgsOpt";
        case Args: return "Args";
        case ArgsTail: return "ArgsTail";
        case Exp: return "Exp";
        case ExpTail: return "ExpTail";
        case AndExp: return "AndExp";
        case AndTail: return "AndTail";
        case EqExp: return "EqExp";
        case EqTail: return "EqTail";
        case RelExp: return "RelExp";
        case RelTail: return "RelTail";
        case AddExp: return "AddExp";
        case AddTail: return "AddTail";
        case MulExp: return "MulExp";
        case MulTail: return "MulTail";
        case Unary: return "Unary";
        case Primary: return "Primary";
        case Call: return "Call";
        case NTermSize: return "NTermSize";
    }
    return "error";
}

std::string TToStr(Terminal t)
{
    switch(t)
    {
    case INT: return "int";
    case BOOL: return "bool";
    case IF: return "if";
    case ELSE: return "else";
    case WHILE: return "while";
    case RETURN: return "return";
    case TRUE: return "true";
    case FALSE: return "false";
    case PRINT: return "print";
    case READ: return "read";
    case IDENT: return "id";
    case NUM: return "num";
    case PLUS: return "+";
    case MINUS: return "-";
    case STAR: return "*";
    case SLASH: return "/";
    case PERCENT: return "%";
    case LT: return "<";
    case LE: return "<=";
    case GT: return ">";
    case GE: return ">=";
    case EQ: return "==";
    case NE: return "!=";
    case AND: return "&&";
    case OR: return "||";
    case NOT: return "!";
    case ASSIGN: return "assign";
    case LPAREN: return "(";
    case RPAREN: return ")";
    case LBRACE: return "{";
    case RBRACE: return "}";
    case COMMA: return ",";
    case SEMI: return ";";
    case EPSILON: return "EPS";
    case _EOF: return "end of file";
    }
    return "";
}
}

inline std::string symToStr(Symbol s)
{
    if(std::holds_alternative<NonTerminal>(s))
        return NTToStr(std::get<NonTerminal>(s));
    else
        return TToStr(std::get<Terminal>(s));
}
