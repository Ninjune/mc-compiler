// File: typechkr.h
#pragma once
#include "../node.h"
#include "../util.h"

struct TypeError {
    int line;
    std::string msg;
};

class TypeChecker {
public:
    typedef STable::Type Type;
    TypeChecker(Node* in) : prog(in) { run(); }

    bool getError() { return err; };
private:
    void run();

    [[noreturn]] static void fail(const Node* n, const std::string& msg);

    static const std::string& idName(const Node* n) { return n->token->value; }
    static const char* name(Type t) { return t == Type::INT ? "int" : "bool"; }

    Type record(Node* n, Type t) { n->type = t; return t; }

    static bool alwaysReturns(const Node* s);

    static bool isT(const Node* n, Terminal t) 
    {
        auto* p = std::get_if<Terminal>(&n->symbol);
        return p && *p == t;
    }

    static bool isNT(const Node* n, NonTerminal nt) 
    {
        auto* p = std::get_if<NonTerminal>(&n->symbol);
        return p && *p == nt;
    }

    static Type typeOf(const Node* typeNode) 
    {
        return isT(typeNode, INT) ? Type::INT : Type::BOOL;
    }

    void expect(Node* e, Type want, const std::string& what) 
    {
        Type got = checkExp(e);

        if (got != want)
            fail(e, what + " must be " + name(want) + " but is " + name(got));
    }

    void checkProgram();
    void checkFun(Node* f);
    void checkStmt(Node* s);
    Type checkExp(Node* e);
    Type checkCall(Node* c);

    Node* prog;
    STable ftable;
    STable vtable;
    Type currentReturn = Type::INT;
    bool err = false;
    std::unordered_map<std::string, std::vector<Type>> paramTypes;
};
