// File: lexer.cpp
#include "lexer.h"
#include "util.h"

static const std::unordered_map<std::string, Token::Kind> keywords = {
    {"int", Token::Kind::INT},   {"bool", Token::Kind::BOOL},
    {"if", Token::Kind::IF},     {"else", Token::Kind::ELSE},
    {"while", Token::Kind::WHILE}, {"return", Token::Kind::RETURN},
    {"true", Token::Kind::TRUE}, {"false", Token::Kind::FALSE},
    {"print", Token::Kind::PRINT}, {"read", Token::Kind::READ},
};
static const std::unordered_map<std::string, Token::Kind> twoCharOps = {
    {"<=", Token::Kind::LE}, {">=", Token::Kind::GE}, {"==", Token::Kind::EQ},
    {"!=", Token::Kind::NE}, {"&&", Token::Kind::AND}, {"||", Token::Kind::OR},
};
static const std::unordered_map<char, Token::Kind> oneCharOps = {
    {'+', Token::Kind::PLUS}, {'-', Token::Kind::MINUS}, {'*', Token::Kind::STAR},
    {'/', Token::Kind::SLASH}, {'%', Token::Kind::PERCENT},
    {'<', Token::Kind::LT}, {'>', Token::Kind::GT}, {'!', Token::Kind::NOT},
    {'=', Token::Kind::ASSIGN}, {'(', Token::Kind::LPAREN}, {')', Token::Kind::RPAREN},
    {'{', Token::Kind::LBRACE}, {'}', Token::Kind::RBRACE},
    {',', Token::Kind::COMMA}, {';', Token::Kind::SEMI},
};


int Lexer::run()
{
    if(readFile()) return 1;

    output = new std::queue<Token>();

    while (true) {
        if (!skipTrivia()) { delete output; output = nullptr; return 1; }
        if (atEnd()) break;
        if (!scanToken()) { delete output; output = nullptr; return 1; }
    }
    addToken(Token::Kind::_EOF);

    return 0;
}

bool Lexer::skipTrivia()
{
    while (!atEnd()) {
        char c = peek();
        if (c == '\n') { line++; curI++; }
        else if (isspace((unsigned char)c)) { curI++; }
        else if (c == '/' && peek(1) == '/')
        {
            while (!atEnd() && peek() != '\n') curI++;   // newline handled next loop
        }
        else if (c == '/' && peek(1) == '*') 
        {
            int startLine = line;
            curI += 2;
            while (!(peek() == '*' && peek(1) == '/')) {
                if (atEnd()) { line = startLine; error("unended comment"); return false; }
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

    if (isalpha((unsigned char)c) || c == '_') {
        while (isalnum((unsigned char)peek()) || peek() == '_') curI++;
        std::string word = input.substr(start, curI - start);
        auto it = keywords.find(word);
        if (it != keywords.end()) addToken(it->second);
        else addToken(Token::Kind::IDENT, word);
        return true;
    }

    if (isdigit((unsigned char)c)) {
        while (isdigit((unsigned char)peek())) curI++;
        std::string digits = input.substr(start, curI - start);
        try { addToken(Token::Kind::NUM, std::to_string(std::stoi(digits))); }
        catch (const std::out_of_range&) { error("integer literal too large: " + digits); return false; }
        return true;
    }

    auto two = twoCharOps.find(input.substr(curI, 2));
    if (two != twoCharOps.end()) { addToken(two->second); curI += 2; return true; }

    auto one = oneCharOps.find(c);
    if (one != oneCharOps.end()) { addToken(one->second); curI++; return true; }

    error(std::string("unexpected character '") + c + "'");
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


void Lexer::addToken(Token::Kind kind, std::string value)
{
    output->push(Token(line, kind, value));
}


void Lexer::error(std::string s)
{
    s = "error: line " + std::to_string(line) + ":\n";

    std::cerr << s << std::endl;
    std::string errorFilePath = extractFileName(inputFilePath) + ".err";
    std::ofstream file(errorFilePath);

    if(!file.is_open())
        std::cerr << "Could not open the input file! Path: " << inputFilePath;

    file << s;

    file.close();
}


