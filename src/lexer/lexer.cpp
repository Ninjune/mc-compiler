// File: lexer.cpp
#include "lexer.h"

static const std::unordered_map<std::string, TokenKind> keywords = {
    {"int", INT},   {"bool", BOOL},
    {"if", IF},     {"else", ELSE},
    {"while", WHILE}, {"return", RETURN},
    {"true", TRUE}, {"false", FALSE},
    {"print", PRINT}, {"read", READ},
};
static const std::unordered_map<std::string, TokenKind> twoCharOps = {
    {"<=", LE}, {">=", GE}, {"==", EQ},
    {"!=", NE}, {"&&", AND}, {"||", OR},
};
static const std::unordered_map<char, TokenKind> oneCharOps = {
    {'+', PLUS}, {'-', MINUS}, {'*', STAR},
    {'/', SLASH}, {'%', PERCENT},
    {'<', LT}, {'>', GT}, {'!', NOT},
    {'=', ASSIGN}, {'(', LPAREN}, {')', RPAREN},
    {'{', LBRACE}, {'}', RBRACE},
    {',', COMMA}, {';', SEMI},
};


int Lexer::run()
{
    if(readFile()) return 1;

    output = new std::queue<Token>();

    while (true)
    {
        if (!skipTrivia()) { delete output; output = nullptr; return 1; }
        if (atEnd()) break;
        if (!scanToken()) { delete output; output = nullptr; return 1; }
    }
    addToken(_EOF);

    return 0;
}

bool Lexer::skipTrivia()
{
    while (!atEnd())
    {
        char c = peek();
        if (c == '\n') { line++; curI++; }
        else if (isspace((unsigned char)c)) { curI++; }
        else if (c == '/' && peek(1) == '/')
            while (!atEnd() && peek() != '\n') curI++;   // newline handled next loop
        else if (c == '/' && peek(1) == '*') 
        {
            int startLine = line;
            curI += 2;
            while (!(peek() == '*' && peek(1) == '/'))
            {
                if (atEnd()) { line = startLine; error("unended comment", line); return false; }
                if (peek() == '\n') line++;
                curI++;
            }
            curI += 2;
        }
        else break;
    }
    return true;
}

bool Lexer::scanToken()
{
    unsigned start = curI;
    char c = peek();

    if (isalpha((unsigned char)c) || c == '_')
    {
        while (isalnum((unsigned char)peek()) || peek() == '_') curI++;
        std::string word = input.substr(start, curI - start);
        auto it = keywords.find(word);
        if (it != keywords.end()) addToken(it->second);
        else addToken(IDENT, word);
        return true;
    }

    if (isdigit((unsigned char)c))
    {
        while (isdigit((unsigned char)peek())) curI++;
        std::string digits = input.substr(start, curI - start);
        try { addToken(NUM, std::to_string(std::stoi(digits))); }
        catch (const std::out_of_range&) { error("integer literal too large: " + digits, line); return false; }
        return true;
    }

    auto two = twoCharOps.find(input.substr(curI, 2));
    if (two != twoCharOps.end()) { addToken(two->second); curI += 2; return true; }

    auto one = oneCharOps.find(c);
    if (one != oneCharOps.end()) { addToken(one->second); curI++; return true; }

    error(std::string("unexpected character '") + c + "'", line);
    return false;
}


int Lexer::readFile()
{
    std::ifstream file(inputFilePath);

    if (!file.is_open())
    {
        std::cerr << "Could not open the input file! Path: " << inputFilePath;
        return 1;
    }

    std::stringstream ss; 

    ss << file.rdbuf();
    input = ss.str();

    file.close();

    return 0;
}


void Lexer::addToken(TokenKind kind, std::string value)
{
    output->push(Token(line, kind, value));
}


