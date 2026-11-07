// if there are compile time issues, use the tables in grammar
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <optional>
#include "grammar.h"
#include "../util.h"


class Node
{
public:
    Node(int line, Symbol s)
        : lineNum(line), symbol(s) {}

    ~Node()
    {
        for(unsigned int i = 0; i < children.size(); i++)
            delete children[i];
    }

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    std::vector<Node*> children;
    int lineNum;
    Symbol symbol;
    std::optional<Token> token; // only has if its a terminal
};


struct SyntaxError { std::string msg; int line; };


class Parser {
public:
    Parser(std::queue<Token>* in) : input(in) { output = parse(); }

    Node* getTree() { return output; } // caller owns the tree
    std::string getString() { return getString(output); }
private:
    std::string getString(Node* node);
    Node* parse();

    // token helpers
    const Token& peek() const { return input->front(); }
    Token next();
    Token expect(Terminal k);
    void fail(const std::string& expected);

    std::string nodeToStr(Node* n);

    // tree helpers
    Node* leaf(const Token& t);
    Node* node(NonTerminal nt, int line, const std::vector<Node*>& kids);

    // one function per nonterminal, in grammar order
    Node* parseS();
    Node* parseProgram();
    void  parseFunDecls(Node* parent);
    Node* parseFunDecl();
    Node* parseParamsOpt();
    void  parseParams(Node* parent);
    void  parseParamsTail(Node* parent);
    Node* parseParam();
    Node* parseType();
    Node* parseBlock();
    void  parseItems(Node* parent);
    Node* parseItem();
    Node* parseDecl();
    Node* parseStmt();
    Node* parseStmtRest(Node* name);
    Node* parseElsePart();
    Node* parseArgsOpt();
    void  parseArgs(Node* parent);
    void  parseArgsTail(Node* parent);
    Node* parseExp();
    Node* parseExpTail(Node* lhs);
    Node* parseAndExp();
    Node* parseAndTail(Node* lhs);
    Node* parseEqExp();
    Node* parseEqTail(Node* lhs);
    Node* parseRelExp();
    Node* parseRelTail(Node* lhs);
    Node* parseAddExp();
    Node* parseAddTail(Node* lhs);
    Node* parseMulExp();
    Node* parseMulTail(Node* lhs);
    Node* parseUnary();
    Node* parsePrimary();
    Node* parseCallRest(Node* name);

    bool holdsLeaf(const Node* n);
    std::string spell(Terminal k);
    std::string spell(const Token& t);
   
    std::queue<Token>* input;
    Node* output;
};
