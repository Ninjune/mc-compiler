// File: lexer.h
#pragma once
#include <queue>
#include <string>
#include <utility>
#include "../token.h"


class Lexer
{
public:
    Lexer(std::string path)
        : inputFilePath(std::move(path))
    {
        run();
    }

    std::queue<Token>* getTokens() { return output; };
private:
    int run();
    int readFile(); // populates input

    [[nodiscard]] char peek(int ahead = 0) const
    {
        return curI + ahead < input.size() ? input[curI + ahead] : '\0';
    }
    [[nodiscard]] bool atEnd() const { return curI >= input.size(); }
    bool skipTrivia(); // whitespace + comments; false on error
    bool scanToken();  // one token; false on error

    void addToken(TokenKind kind, const std::string& value = "");

    std::string inputFilePath;
    int line = 1;
    unsigned int curI = 0;

    std::string input;
    std::queue<Token>* output;
};
