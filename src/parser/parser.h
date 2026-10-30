// File: parser.h
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include "grammar.h"


class ASTNode
{
public:
    ASTNode *left, *right;
    int lineNum;
    Symbol symbol;
};

class Parser {
public:
    Parser(std::queue<Token>* in)
        : input(in) { genTree(); }

    ASTNode* getRoot() { return output; }
private:
    void genTree();

    std::queue<Token>* input;
    ASTNode* output;
};

