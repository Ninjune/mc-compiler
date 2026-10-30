// File: main.cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "util.h"
std::string parse_flag(char* arg);


int main(int argc, char** argv)
{
    std::string input_file = "tests/part1/bad_char.mc";
    std::unordered_set<std::string> flags;

    // Parse arguments
    argc--; // ignore first arg
    if(argc < 1)
    {
        std::cerr << "Invalid arguments!" << std::endl;

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

        std::string tokensPath = extractFileName(input_file) + ".out";
        std::ofstream file(tokensPath);

        if(!file.is_open())
            std::cerr << "Could not open the input file! Path: " << tokensPath;

        file << output;

        file.close();
    }
    else if (flags.count("tables"))
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
    }


    // TODO: ...

    return 0;
}


std::string parse_flag(char* arg)
{
    std::string temp;

    for(int i = 2; arg[i] != '\0'; i++)
        temp += arg[i];

    return temp;
}
