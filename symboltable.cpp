#include "symboltable.h"


SymbolTable::SymbolTable() {
    addToken("int", TokenType::KEYWORD, TokenValue::INT);
    addToken("double", TokenType::KEYWORD, TokenValue::DOUBLE);
    //分支与循环
    addToken("if", TokenType::OPREATOR, TokenValue::IF);
    addToken("else", TokenType::OPREATOR, TokenValue::ELSE);
    addToken("for", TokenType::OPREATOR, TokenValue::FOR);
    addToken("while", TokenType::OPREATOR, TokenValue::WHILE);

    //算术运算
    addToken("+", TokenType::OPREATOR, TokenValue::ADD);
    addToken("-", TokenType::OPREATOR, TokenValue::SUB);
    addToken("*", TokenType::OPREATOR, TokenValue::MUL);
    addToken("/", TokenType::OPREATOR, TokenValue::DIV);
    addToken("%", TokenType::OPREATOR , TokenValue::MOD);
    addToken("=", TokenType::OPREATOR, TokenValue::ASSIGN);
    addToken("++", TokenType::OPREATOR, TokenValue::INC);
    addToken("--", TokenType::OPREATOR, TokenValue::DEC);

    //位运算
    addToken("<<", TokenType::OPREATOR, TokenValue::LSHIFT);
    addToken(">>", TokenType::OPREATOR, TokenValue::RSHIFT);
    addToken("~", TokenType::OPREATOR, TokenValue::BITNOT);
    addToken("&", TokenType::OPREATOR, TokenValue::BITAND);
    addToken("|", TokenType::OPREATOR, TokenValue::BITOR);

    //逻辑运算
    addToken("&&", TokenType::OPREATOR, TokenValue::AND);
    addToken("||", TokenType::OPREATOR, TokenValue::OR);
    addToken("!", TokenType::OPREATOR, TokenValue::NOT);

    //比较
    addToken("==", TokenType::OPREATOR, TokenValue::EQUAL);
    addToken("!=", TokenType::OPREATOR, TokenValue::NOT_EAUAL);
    addToken(">", TokenType::OPREATOR, TokenValue::GREATER);
    addToken("<", TokenType::OPREATOR, TokenValue::LESS);
    addToken(">=", TokenType::OPREATOR, TokenValue::NOT_LESS);
    addToken("<=", TokenType::OPREATOR, TokenValue::NOT_GREATER);

    //括号与逗号
    addToken("(", TokenType::DELIMITER, TokenValue::LPAR);
    addToken(")", TokenType::DELIMITER, TokenValue::RPAR);
    addToken("{", TokenType::DELIMITER, TokenValue::LBRACE);
    addToken("}", TokenType::DELIMITER, TokenValue::RBRACE);
    addToken(",", TokenType::DELIMITER, TokenValue::COMMA);
    addToken(";", TokenType::DELIMITER, TokenValue::SEMI);

    addToken("output", TokenType::KEYWORD , TokenValue::OUTPUT);
    addToken("input", TokenType::KEYWORD , TokenValue::INPUT);

    addToken("END_OF_FILE", TokenType::END_OF_FILE, TokenValue::UNDEFINE);
}

void SymbolTable::addToken(const std::string &name, TokenType tokenType, TokenValue tokenValue) {
    symbolTable.insert({name, std::make_tuple(tokenType, tokenValue)});
}

std::tuple<TokenType, TokenValue> SymbolTable::findToken(const std::string &name) {
    auto res = symbolTable.find(name);
    if (res != symbolTable.end()) {
        return res->second;
    }
    return std::make_tuple(TokenType::UNKOWN, TokenValue::UNDEFINE);
}

bool SymbolTable::haveToken(const std::string &name) {
    return symbolTable.find(name) != symbolTable.end();
}