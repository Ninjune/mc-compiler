// File: main.cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include "lexer.h"
std::string parse_flag(char* arg);


int main(int argc, char** argv)
{
    std::string input_file;
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
        return Lexer::getInstance().run(input_file);
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
