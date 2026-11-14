// File: typechkr.h
#pragma once
#include "../parser/parser.h"

class TypeChkr {
public:
    TypeChkr(Node* in) : tree(in) { run(); }

    Node* getAnnotatedTree() { return tree; };
private:
    void run();
    void recurse(Node* tree, STable* ftable, STable* vtable);

    Node* tree;
};
