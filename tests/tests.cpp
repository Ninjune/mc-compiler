// File: tests.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include "../src/lexer.h"
std::string readFile(std::string inputFilePath);
int test(int i, std::string inputFilePath, std::string expectedFilePath);


int main()
{
    test(1, "tests/part1/tokens.mc", "tests/part1/tokens.out");
    
    return 0;
}


int test(int i, std::string inputFilePath, std::string expectedFilePath)
{
    std::cout << "test " << i << " begin.\n";

    std::queue<Token>* tokens = Lexer(inputFilePath).getTokens();
    std::string expectedOutput = readFile(expectedFilePath);

    if(tokens == nullptr)
    {
        std::cerr << "test 1 failed. tokens was nullptr.";
        return 1;
    }

    std::string tokensString = "";

    while(!tokens->empty())
    {
        const Token& t = tokens->front();
        tokensString += std::to_string(t.line) + " " + t.kindName();
        if(t.value != "")
            tokensString += " " + t.value;
        tokensString += "\n";
        tokens->pop();
    }

    delete tokens;

    if(tokensString == expectedOutput)
        std::cout << "test " << i << " passed.\n";
    else 
        std::cout << "test " << i << " failed.\n";

    return 0;
}


std::string readFile(std::string inputFilePath)
{
    std::ifstream file(inputFilePath);

    if (!file.is_open())
    {
        std::cerr << ("Could not open the input file! Path: " + inputFilePath);
        return "";
    }

    std::stringstream ss; ss << file.rdbuf();

    file.close();

    return ss.str();
}
