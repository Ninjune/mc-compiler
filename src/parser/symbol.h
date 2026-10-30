// File: symbol.h
#pragma once
#include "../token.h"
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
    CallRest,
    NTermSize
};

typedef TokenKind Terminal;
typedef std::variant<NonTerminal, Terminal> Symbol;

inline std::string NTToStr(NonTerminal nt)
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
        case CallRest: return "CallRest";
        case NTermSize: return "NTermSize";
    }
    return "error";
}

inline std::string symToStr(Symbol s)
{
    if(std::holds_alternative<NonTerminal>(s))
        return NTToStr(std::get<NonTerminal>(s));
    else
        return kindName(std::get<Terminal>(s));
}
