// File: lexer.h
#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include "token.h"


class Lexer {
public:
    Lexer(std::string path)
        : inputFilePath(path) { run(); }

    std::queue<Token>* getTokens() { return output; };
private:
    int run();
    int readFile(); // populates input

    void error(std::string s);

    char peek(int ahead = 0) const { return curI + ahead < input.size() ? input[curI + ahead] : '\0'; }
    bool atEnd() const { return curI >= input.size(); }
    bool skipTrivia();     // whitespace + comments; false on error
    bool scanToken();      // one token; false on error

    void addToken(Token::Kind kind, std::string value = "");

    std::string inputFilePath;
    int line = 1;
    unsigned int curI = 0;

    std::string input;
    std::queue<Token>* output;
};
