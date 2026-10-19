// File: lexer.cpp
#include "lexer.h"


int Lexer::run(std::string input_file_path) 
{
    std::string input = read_file(input_file_path);

    std::stack<Token> tokens;

    return 0;
}


std::string Lexer::read_file(std::string input_file_path)
{
    std::ifstream file(input_file_path);
    if (!file.is_open()) {
        throw std::invalid_argument("(Manually invoked) Error: Could not open the file! Path: " + input_file_path);
    }

    std::string input;
    std::string line;
    while (std::getline(file, line)) {
        input += line;
    }

    file.close();

    return input;
}
