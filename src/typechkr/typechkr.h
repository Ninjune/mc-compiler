// File: typechkr.h
#pragma once
#include "../node.h"

struct TypeError {
    int line;
    std::string msg;
};

class TypeChecker {
public:
    enum class Type {
        INT,
        BOOL
    };
    TypeChecker(Node* in) : prog(in) { run(); }

    bool getError() { return err; };
private:
    void run();

    [[noreturn]] static void fail(const Node* n, const std::string& msg);

    static const std::string& idName(const Node* n) { return n->token->value; }
    static const char* name(Type t) { return t == Type::INT ? "int" : "bool"; }

    static bool alwaysReturns(const Node* s);

    static Type typeOf(const Node* typeNode) 
    {
        return typeNode->isSym(INT) ? Type::INT : Type::BOOL;
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
    STable<Type> ftable;
    STable<Type> vtable;
    Type currentReturn = Type::INT;
    bool err = false;
    std::unordered_map<std::string, std::vector<Type>> paramTypes;
};
