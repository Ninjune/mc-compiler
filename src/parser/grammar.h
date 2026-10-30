// File: grammar.h
#pragma once
#include "symbol.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>


typedef NonTerminal RuleLHS;
typedef std::set<std::vector<Symbol>> RuleRHS;
typedef std::pair<RuleLHS, RuleRHS> Rule;
typedef std::unordered_map<RuleLHS, RuleRHS> Productions;
typedef std::unordered_set<Symbol> NullableSet;
typedef std::unordered_map<Symbol, std::unordered_set<Terminal>> FirstMap;
typedef std::unordered_map<Symbol, std::unordered_set<Terminal>> FollowMap;
typedef std::unordered_map<NonTerminal, std::unordered_map<Terminal, RuleRHS>> ParseMap;

class Grammar {
public:
    Grammar(Productions rules)
        : rules(rules), nullable(genNullable()),
        first(genFirst()), follow(genFollow()),
        parse(genParse()) { }

    Productions getRules() const { return rules; }
    NullableSet getNullable() const { return nullable; }
    FirstMap getFirst() const { return first; }
    FollowMap getFollow() const { return follow; }
    ParseMap getParse() const { return parse; }
private:
    NullableSet genNullable() const
    {
        NullableSet nullables;

        bool changed = true;
        while(changed)
        {
            changed = false;
            for(unsigned int symbol = S; symbol < NTermSize; symbol++)
            {
                NonTerminal nt = (NonTerminal)symbol;
                // iter through
                for(const auto& rhs : rules.at(nt))
                {
                    bool allNullable = true;
                    for(const auto& s : rhs)
                    {
                        if(!nullables.count(s))
                        {
                            allNullable = false;
                            break;
                        }
                    }
                    if(allNullable && !nullables.count(nt))
                    {
                        nullables.insert(nt);
                        changed = true;
                    }
                }
            }
        }
        return nullables;
    }

    FirstMap genFirst() const
    {
        FirstMap first;

        // initialize terminals to themselves & NTs to empty
        for(int i = 0; i <= _EOF; i++)
            first[(Terminal)i] = {(Terminal)i};

        for(int i = S; i < NTermSize; i++)
            first[(NonTerminal)i] = {};

        bool changed = true;

        // main loop
        while(changed)
        {
            changed = false;
            // iterate through rules
            for(const auto& rule : rules)
            {
                const RuleLHS& lhs = rule.first;
                const RuleRHS& rhs = rule.second;

                // iterate through each alternate rhs
                for(auto& alternate : rhs)
                {
                    // epsilon check
                    bool allNullable = true;

                    for(const Symbol& sym : alternate)
                    if(!nullable.count(sym))
                        allNullable = false;

                    if(!first[lhs].count(EPSILON) && allNullable)
                        if(first[lhs].insert(EPSILON).second)
                            changed = true;

                    // iter through the symbols
                    for(const Symbol& sym : alternate)
                    {
                        if(std::holds_alternative<Terminal>(sym)) // Terminal
                        {
                            const Terminal terminal = std::get<Terminal>(sym);
                            if(first[lhs].insert(terminal).second)
                                changed = true;
                        }
                        else // NT
                    {
                            const NonTerminal nonTerminal = std::get<NonTerminal>(sym);
                            // put the terminals from the NT's first into this first set
                            for(const Terminal terminal : first[nonTerminal])
                            {
                                if(terminal == EPSILON)
                                    continue;
                                if(first[lhs].insert(terminal).second)
                                    changed = true;
                            }
                        }

                        // check nullable
                        if(!nullable.count(sym))
                            break;
                    }
                }

            }
        }

        return first;
    }

    std::set<Terminal> firstOfSymbolList(const std::vector<Symbol>& beta) const
    {
        std::set<Terminal> res;
        for (const Symbol& sym : beta) {
            for (Terminal t : first.at(sym))
            if (t != EPSILON)
                res.insert(t);
            if (!nullable.count(sym)) 
                return res;
        }
        res.insert(EPSILON); // all of beta nullable
        return res;
    }

    FollowMap genFollow() const
    {
        FollowMap follow;

        // initialize follow sets
        for(int i = S; i < NTermSize; i++)
            follow[(NonTerminal)i] = {};

        follow[S] = {_EOF};

        bool changed = true;
        while(changed)
        {
            changed = false;
            for(const auto& rule : rules)
            {
                const RuleLHS& lhs = rule.first;
                const RuleRHS& rhs = rule.second;

                // iterate through each alternate rhs
                for(const auto& alternate : rhs)
                {
                    // for each nonterminal B in the production, update FOLLOW(B)
                    for(size_t i = 0; i < alternate.size(); ++i)
                    {
                        if(!std::holds_alternative<NonTerminal>(alternate[i]))
                            continue;

                        const NonTerminal B = std::get<NonTerminal>(alternate[i]);
                        std::vector<Symbol> beta;

                        for(size_t j = i + 1; j < alternate.size(); ++j)
                            beta.push_back(alternate[j]);

                        auto& followB = follow[B];

                        if(!beta.empty())
                        {
                            // A -> alpha B beta, beta != epsilon:
                            // FOLLOW(B) := FOLLOW(B) U (FIRST(beta) - {epsilon})
                            const std::set<Terminal> betaFirst = firstOfSymbolList(beta);
                            for(const Terminal t : betaFirst)
                            {
                                if(t == EPSILON)
                                    continue;
                                if(followB.insert(t).second)
                                    changed = true;
                            }

                            // epsilon in FIRST(beta):
                            // FOLLOW(B) := FOLLOW(B) U FOLLOW(A)
                            const auto followA = follow[lhs];
                            if(betaFirst.count(EPSILON))
                                for(const Terminal t : followA)
                                    if(followB.insert(t).second)
                                        changed = true;
                        }
                        else // A -> alpha B: FOLLOW(B) := FOLLOW(B) U FOLLOW(A)
                        {
                            const auto followA = follow[lhs];
                            for(const Terminal t : followA)
                                if(followB.insert(t).second)
                                    changed = true;
                        }
                    }
                }
            }
        }

        return follow;
    }

    ParseMap genParse() const 
    {
        // initialize M
        ParseMap M;

        for(int i = S; i < NTermSize; i++)
            for(int j = 0; j <= _EOF; j++)
                M[(NonTerminal)i][(Terminal)j] = {};

        // for each A ∈ N
        for(int i = S; i < NTermSize; i++)
        {
            NonTerminal A = (NonTerminal)i;
            const RuleRHS& alternates = rules.at(A);
            // for each production A → α
            for(const std::vector<Symbol>& alpha : alternates)
            {
                // if a ∈ FIRST(α)
                auto firstAlpha = firstOfSymbolList(alpha);

                // and a != ε
                for(const Terminal& a : firstAlpha)
                    if(a != EPSILON)
                        M[A][a].insert(alpha); // then M[A, a] = M[A, a] ∪ {α}

                if(firstAlpha.count(EPSILON)) // if ε ∈ FIRST(α)
                    for (const Terminal& b : follow.at(A)) // then for each b ∈ FOLLOW(A)
                        M[A][b].insert(alpha); // M[A, b] = M[A, b] ∪ {α}
            }
        }

        return M;
    }

    Productions rules;
    NullableSet nullable;
    FirstMap first;
    FollowMap follow;
    ParseMap parse;
};

static const Grammar grammar({
    {S, {{Program, _EOF}}},
    {Program, {{FunDecls}}},
    {FunDecls, {{FunDecl, FunDecls}, {}}},
    {FunDecl, {{Type, IDENT, LPAREN, ParamsOpt, RPAREN, Block}}},
    {ParamsOpt, {{Params}, {}}},
    {Params, {{Param, ParamsTail}}},
    {ParamsTail, {{COMMA, Param, ParamsTail}, {}}},
    {Param, {{Type, IDENT}}},
    {Type, {{INT}, {BOOL}}},
    {Block, {{LBRACE, Items, RBRACE}}},
    {Items, {{Item, Items}, {}}},
    {Item, {{Decl}, {Stmt}}},
    {Decl, {{Type, IDENT, ASSIGN, Exp, SEMI}}},
    {Stmt, {
        {IDENT, StmtRest}, 
        {IF, LPAREN, Exp, RPAREN, Stmt, ElsePart}, 
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
    {ArgsOpt, {{Args}, {}}},
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
