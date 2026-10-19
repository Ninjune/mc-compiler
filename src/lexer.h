// File: lexer.h
#pragma once
#include <iostream>
#include <fstream>
#include "token.h"


// singleton
class Lexer {
public:
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    Lexer(Lexer&&) = delete;
    Lexer& operator=(Lexer&&) = delete;

    static Lexer& getInstance() {
        static Lexer instance; 
        return instance;
    }

    int run(std::string input_file_path);
private:
    std::string read_file(std::string input_file_path);

    Lexer() { }
    ~Lexer() = default;
};
