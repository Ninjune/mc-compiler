#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <optional>
#include "grammar.h"
#include "../util.h"

class BTNode
{
public:
    BTNode(int line, Symbol s)
        : lineNum(line), symbol(s) {}
    ~BTNode()
    {
        delete left;
        delete right;
    }
    BTNode(const BTNode&) = delete;
    BTNode& operator=(const BTNode&) = delete;

    BTNode *left = nullptr;
    BTNode *right = nullptr;
    int lineNum;
    Symbol symbol;
    std::optional<Token> token;
};

class Parser {
public:
    Parser(std::queue<Token>* in)
        : input(in) { output = genTree(); }

    BTNode* getTree() { return output; } // caller owns the tree
private:
    struct StackEntry { Symbol sym; BTNode* node; };

    BTNode* genTree();
    BTNode* CSTToAST(BTNode* ast);
    BTNode* failGen(BTNode* root, const std::string& msg, int line);
    StackEntry TopOfStack() { return stack.top(); }
    Token NextToken();
    void Push(const std::vector<Symbol>& sl, BTNode* parent, int line);
    void Pop() { stack.pop(); }

    std::queue<Token>* input;
    BTNode* output;

    std::stack<StackEntry> stack;
    ParseMap parseTable = grammar.getParse();
};
