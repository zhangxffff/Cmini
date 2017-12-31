#ifndef TOKEN_H
#define TOKEN_H

#include<string>
#include <stdbool.h>

enum class TokenType {
    INTEGER,
    REAL,
    CHAR,
    STRING,
    IDENTIFIER,
    OPREATOR,
    KEYWORD,
    DELIMITER,
    END_OF_FILE,
    UNKOWN
};

enum class TokenValue {
    INT,
    DOUBLE,
    //分支与循环

    IF,
    ELSE,
    FOR,
    WHILE,

    //算术运算

    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    ASSIGN,
    DEC,
    INC,

    //逻辑运算

    AND,
    OR,
    NOT,


    //位运算

    LSHIFT,
    RSHIFT,
    BITNOT,
    BITAND,
    BITOR,

    //比较

    EQUAL,
    NOT_EAUAL,
    GREATER,
    LESS,
    NOT_GREATER,
    NOT_LESS,

    //括号与逗号

    LPAR,
    RPAR,
    LBRACE,
    RBRACE,
    COMMA,
    SEMI,

    ID,
    INTNUM,
    DOUBLENUM,
    STRING,
    UNDEFINE,

    OUTPUT,
    INPUT,

    TERMINAL_NUM,
    NONE
};

class Token {
private:
    long line;
    std::string name;
    TokenType type;
    TokenValue value;

    int intValue;
    double realValue;
    std::string strValue;

public:


    Token() {
        line = 0;
    }


    long getLine() const {
        return line;
    }

    void setLine(long line) {
        Token::line = line;
    }


    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        Token::name = name;
    }

    TokenType const &getType() const {
        return type;
    }

    void setType(TokenType const &type) {
        Token::type = type;
    }

    TokenValue const &getValue() const {
        return value;
    }

    void setValue(TokenValue const &value) {
        Token::value = value;
    }

    int getIntValue() const {
        return intValue;
    }

    void setIntValue(int intValue) {
        Token::intValue = intValue;
    }

    double getRealValue() const {
        return realValue;
    }

    void setRealValue(double realValue) {
        Token::realValue = realValue;
    }

    const std::string &getStrValue() const {
        return strValue;
    }

    void setStrValue(std::string &strValue) {
        Token::strValue = strValue;
    }

    void setToken(long line, std::string &name, TokenType const &type, TokenValue const &value, int intValue, double realValue, std::string &strValue) {
        Token::line = line;
        Token::name = name;
        Token::type = type;
        Token::value = value;
        Token::intValue = intValue;
        Token::realValue = realValue;
        Token::strValue = strValue;
    }
};



#endif