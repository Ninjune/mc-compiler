// File: symbol.h
#pragma once
#include "../token.h"
#include <unordered_map>
#include <variant>
#include <vector>

enum NTermSymbol
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

typedef TokenKind TermSymbol;
typedef std::variant<NTermSymbol, TermSymbol> Symbol;
