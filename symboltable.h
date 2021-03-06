#ifndef SYMBOL_H
#define SYMBOL_H

#include "token.h"
#include<unordered_map>
#include<tuple>
#include<string>

class SymbolTable {
private:
    std::unordered_map<std::string, std::tuple<TokenType, TokenValue>> symbolTable;
public:
    SymbolTable();

    std::tuple<TokenType, TokenValue> findToken(const std::string &name);

    void addToken(const std::string &name, TokenType tokenType, TokenValue tokenValue);

    bool haveToken(const std::string &name);
};

#endif