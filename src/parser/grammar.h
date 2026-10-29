// File: grammar.h
#pragma once
#include "symbol.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>


typedef NTermSymbol RuleLHS;
typedef std::set<std::vector<Symbol>> RuleRHS;
typedef std::unordered_map<RuleLHS, RuleRHS> Rules;
typedef std::unordered_set<NTermSymbol> NullableSet;
typedef std::unordered_map<Symbol, std::unordered_set<TermSymbol>> FirstMap;
typedef std::unordered_map<Symbol, std::unordered_set<TermSymbol>> FollowMap;

class Grammar {
public:
    Grammar(Rules rules)
        : rules(rules) { nullable = genNullable(); }
    Rules rules;

    FirstMap first() const
    {
        FirstMap first;

        for(int i = 0; i <= _EOF; i++)
            first[(TermSymbol)i] = {(TermSymbol)i};

        for(int i = S; i < NTermSize; i++)
            first[(NTermSymbol)i] = {};

        bool changed = true;
        while(changed)
        {
            changed = false;
            // ...
        }

        return first;
    }

    NullableSet getNullable() const { return nullable; }
private:
    NullableSet genNullable() const
    {
        std::unordered_set<NTermSymbol> nullables;

        // a nonterminal is nullable if any of its productions is entirely nullable
        for(unsigned int symbol = S; symbol < NTermSize; symbol++)
            if(isNullable((NTermSymbol)symbol))
                nullables.insert((NTermSymbol)symbol);

        return nullables;
    }

    bool isNullable(NTermSymbol symbol) const
    {
        // each element of the RHS set is a production
        for(auto& rhs : rules.at((RuleLHS)symbol))
        {
            bool allNullable = true;
            // iterate through all symbols in the rhs
            for(auto& s : rhs)
            {
                // if s in T
                if(!std::holds_alternative<NTermSymbol>(s))
                {
                    allNullable = false;
                    break;
                }
                // if s in NT and it is not nullable
                if(!isNullable(std::get<NTermSymbol>(s)))
                {
                    allNullable = false;
                    break;
                }
            }
            if(allNullable)
                return true;
        }
        return false;
    }

    NullableSet nullable;
};

static const Grammar grammar({
    {S, {{Program, _EOF}}},
    {Program, {{FunDecls}}},
    {FunDecls, {{FunDecl, FunDecls}, {}}},
    {FunDecl, {{Type, IDENT, LPAREN, ParamsOpt, RPAREN, Block}}},
    {ParamsOpt, {{Params, {}}, {}}},
    {Params, {{Param, ParamsTail}}},
    {ParamsTail, {{COMMA, Param, ParamsTail}, {}}},
    {Param, {{Type, IDENT}}},
    {Type, {{INT, BOOL}}},
    {Block, {{LBRACE, Items, RBRACE}}},
    {Items, {{Item, Items}, {}}},
    {Item, {{Decl, Stmt}}},
    {Decl, {{Type, IDENT, ASSIGN, Exp, SEMI}}},
    {Stmt, {
        {IDENT, StmtRest}, 
        {IF, LPAREN, Exp, RPAREN, Stmt, ELSE, ElsePart}, 
        {WHILE, LPAREN, Exp, RPAREN, Stmt}, 
        {RETURN, Exp, SEMI}, 
        {PRINT, LPAREN, Exp, RPAREN, SEMI}, 
        {Block}
    }},
    {StmtRest, {
        {ASSIGN, Exp, SEMI},
        {LPAREN, ArgsOpt, RPAREN, SEMI}
    }},
    {ElsePart, {{ELSE, Stmt}, {}}},
    {ArgsOpt, {{Args, {}}, {}}},
    {Args, {{Exp, ArgsTail}}},
    {ArgsTail, {{COMMA, Exp, ArgsTail}, {}}},
    {Exp, {{AndExp, ExpTail}}},
    {ExpTail, {{OR, AndExp, ExpTail}, {}}},
    {AndExp, {{EqExp, AndTail}}},
    {AndTail, {{AND, EqExp, AndTail}, {}}},
    {EqExp, {{RelExp, EqTail}}},
    {EqTail, {{EQ, RelExp}, {NE, RelExp}, {}}},
    {RelExp, {{AddExp, RelTail}}},
    {RelTail, {
        {LT, AddExp}, 
        {LE, AddExp}, 
        {GT, AddExp}, 
        {GE, AddExp}, 
        {}
    }},
    {AddExp, {{MulExp, AddTail}}},
    {AddTail, {
        {PLUS, MulExp, AddTail}, 
        {MINUS, MulExp, AddTail}, 
        {}
    }},
    {MulExp, {{Unary, MulTail}}},
    {MulTail, {
        {STAR, Unary, MulTail},
        {SLASH, Unary, MulTail},
        {PERCENT, Unary, MulTail}, 
        {}
    }},
    {Unary, {
        {MINUS, Unary}, 
        {NOT, Unary}, 
        {Primary}
    }},
    {Primary, {
        {NUM}, 
        {TRUE}, 
        {FALSE}, 
        {IDENT, CallRest}, 
        {READ, LPAREN, RPAREN}, 
        {LPAREN, Exp, RPAREN}
    }},
    {CallRest, {{LPAREN, ArgsOpt, RPAREN}, {}}},
});
