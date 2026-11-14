// if there are compile time issues, use the tables in grammar
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include "../util.h"
#include "../node.h"


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
    void  parseArgsOpt(Node* parent);
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
    Node* parseCall(Node* name);

    bool holdsLeaf(const Node* n);
    std::string spell(Terminal k);
    std::string spell(const Token& t);
   
    std::queue<Token>* input;
    Node* output;
};
