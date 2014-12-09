#include "symbol.h"
#include <iostream>
#include <fstream>

#ifndef LEXER_H
#define LEXER_H


class Lexer{
private:
    long line;
    Token token;
    Symbol symbol;
    std::string fileName;
    std::ifstream input;
    std::string buffer;

public:

    Lexer(std::string &fileName) : symbol(), fileName(fileName) {
        line = 1;
        input.open(fileName);
    }

    Token getNextToken();
    void setNextToken();
    void handleNum();
    void handleIdentifier();
    void handleOther();
    void makeToken(std::string name, TokenType type, TokenValue value);
    void makeToken(std::string name, TokenType type, TokenValue value, int intValue);
    void makeToken(std::string name, TokenType type, TokenValue value, double realValue);
    void makeToken(std::string name, TokenType type, TokenValue value, std::string strValue);
    void handleComment();
    void error();

    Token const &getToken() const {
        return token;
    }
};

#endif