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

    std::cout << "First: \n";
    for(auto& r : grammar.getFirst())
    {
        std::cout << symToStr(r.first) << ": {";
        for(auto& a : r.second)
            std::cout << symToStr(a) << ", ";
        std::cout << "}\n";
    }
    std::cout << "\nFollow: \n";
    for(auto& r : grammar.getFollow())
    {
        std::cout << symToStr(r.first) << ": {";
        for(auto& a : r.second)
            std::cout << symToStr(a) << ", ";
        std::cout << "}\n";
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
