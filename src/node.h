// File: node.h
#pragma once
#include <optional>
#include "grammar.h"
#include "stable.h"

class Node
{
public:
    Node(int line, Symbol s)
        : lineNum(line), symbol(s) {}

    ~Node()
    {
        for(unsigned int i = 0; i < children.size(); i++)
            delete children[i];
        delete vtable;
    }

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    std::vector<Node*> children;
    int lineNum;
    Symbol symbol;
    std::optional<Token> token; // only has if its a terminal
    std::optional<STable::Type> type;

    STable* ftable = nullptr;
    STable* vtable = nullptr;
};
