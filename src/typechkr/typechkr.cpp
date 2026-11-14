// File: typechkr.cpp
#include "typechkr.h"


// goal: make vtable and ftable point to the vtable/ftable at each node
void TypeChkr::run()
{
    STable* ftable = new STable();

    recurse(tree, ftable);
}


void TypeChkr::recurse(Node* tree, STable* ftable, STable* vtable)
{
    tree->ftable = ftable;
    tree->vtable = new STable(vtable);

    for(Node* child : tree->children)
    {

        recurse(child, ftable, tree->vtable);
    }
}
