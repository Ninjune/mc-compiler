// File: main.cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include "ir/interp.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "astgraph/astgraph.h"
#include "typechkr/typechkr.h"
#include "ir/ir.h"
std::string parse_flag(char* arg);
int tokens(std::string input_file);
int tables();
int ast(std::string input_file);
int check(std::string input_file);
int ir(std::string input_file);
int runir(std::string input_file);
int ir(std::string input_file);
int runir(std::string input_file);


int main(int argc, char** argv)
{
    std::string input_file = "tests/part1/bad_char.mc";
    std::unordered_set<std::string> flags;

    // Parse arguments
    argc--; // ignore first arg
    if(argc < 1)
    {
        std::cerr << "Invalid arguments!" << "\n";

        return 1;
    }

    for(int i = 1; i <= argc; i++)
    {
        if(argv[i][0] == '-' && argv[i][1] == '-')
            flags.insert(parse_flag(argv[i]));
        else
            input_file = argv[i];
    }

    // Run program based on flags
    if(flags.count("tokens"))
        return tokens(input_file);
    if (flags.count("tables"))
        return tables();
    if (flags.count("ast"))
        return ast(input_file);
    if (flags.count("check"))
        return check(input_file);
    if (flags.count("ir"))
        return ir(input_file);
    if (flags.count("run-ir"))
        return runir(input_file);

    // TODO: ...

    return 0;
}

int tokens(std::string input_file)
{
    std::queue<Token>* tokens = Lexer(input_file).getTokens();

    if(tokens == nullptr)
        return 1;

    std::string output = "";
    while(!tokens->empty())
    {
        const Token& t = tokens->front();

        std::string line;
        line = std::to_string(t.line) + " " + t.kindName();
        if(t.value != "")
            line += " " + t.value;
        line += "\n";

        output += line;

        tokens->pop();
    }

    delete tokens;

    std::cout << output << "\n";
    return 0;
}

int tables()
{
    std::cout << "First: \n";
    for(auto& r : grammar.getFirst())
    {
        std::cout << symToStr(r.first) << ": {";
        bool first = true;
        for(auto& a : r.second)
        {
            if(!first) std::cout << ", ";
            std::cout << symToStr(a);
            first = false;
        }
        std::cout << "}\n";
    }

    std::cout << "\nFollow: \n";
    for(auto& r : grammar.getFollow())
    {
        std::cout << symToStr(r.first) << ": {";
        bool first = true;
        for(auto& a : r.second)
        {
            if(!first) std::cout << ", ";
            std::cout << symToStr(a);
            first = false;
        }
        std::cout << "}\n";
    }

    std::cout << "\nParse table: \n";
    for(auto& nTerm : grammar.getParse())
    {
        std::string A = symToStr(nTerm.first);
        for(auto& term : nTerm.second)
        {
            if(term.second.empty()) continue;
            std::string a = symToStr(term.first);

            std::cout << "M(" << A << ", " << a << "): {";
            bool firstAlt = true;
            for(auto& alternate : term.second)
            {
                if(!firstAlt) std::cout << ", ";
                std::cout << "[";
                bool firstSym = true;

                if(alternate.empty())
                    std::cout << "ε";

                for(auto& sym : alternate)
                {
                    if(!firstSym) std::cout << " ";
                    std::cout << symToStr(sym);
                    firstSym = false;
                }
                std::cout << "]";
                firstAlt = false;
            }
            std::cout << "}\n";
        }
    }
    return 0;
}

int ast(std::string input_file)
{
    auto* tokens = Lexer(input_file).getTokens();

    if(tokens == nullptr)
        return 1;

    Parser p = Parser(tokens);
    Node* tree = p.getTree();

    if(tree == nullptr)
        return 1;

    #ifdef BUILD_GRAPHVIZ
    if(!ASTGraph::render(tree, "ast.svg", "svg"))
        std::cerr << "failed to write ast.svg\n";
    #endif

    std::cout << p.getString() << std::endl;

    delete tree;

    return 0;
}

int check(std::string input_file)
{
    auto* tokens = Lexer(input_file).getTokens();

    if(tokens == nullptr)
        return 1;

    Node* tree = Parser(tokens).getTree();

    if(tree == nullptr)
        return 1;

    bool err = TypeChecker(tree).getError();

    if(!err)
        std::cout << "OK\n";
    else
        return 1;

    delete tree;

    return 0;
}

int ir(std::string input_file)
{
    auto* tokens = Lexer(input_file).getTokens();

    if(tokens == nullptr)
        return 1;

    Node* tree = Parser(tokens).getTree();

    if(tree == nullptr)
        return 1;

    bool err = TypeChecker(tree).getError();

    if(err)
        return 1;

    auto* irprog = IR(tree).program();

    std::cout << irprog->toString();

    delete irprog;
    delete tree;
    return 0;
}

int runir(std::string input_file)
{
    auto* tokens = Lexer(input_file).getTokens();

    if(tokens == nullptr)
        return 1;

    Node* tree = Parser(tokens).getTree();

    if(tree == nullptr)
        return 1;

    bool err = TypeChecker(tree).getError();

    if(err)
        return 1;

    auto* irprog = IR(tree).program();

    IRInterp(*irprog).run();

    delete irprog;
    delete tree;

    return 0;
}

std::string parse_flag(char* arg)
{
    std::string temp;

    for(int i = 2; arg[i] != '\0'; i++)
        temp += arg[i];

    return temp;
}
