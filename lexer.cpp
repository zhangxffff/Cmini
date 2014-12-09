#include "lexer.h"


void Lexer::error() {
}

void Lexer::makeToken(std::string name, TokenType type, TokenValue value) {
    token.setLine(line);
    token.setName(name);
    token.setType(type);
    token.setValue(value);
}

void Lexer::makeToken(std::string name, TokenType type, TokenValue value, int intValue) {
    makeToken(name, type, value);
    token.setIntValue(intValue);
}

void Lexer::makeToken(std::string name, TokenType type, TokenValue value, double realValue) {
    makeToken(name, type, value);
    token.setRealValue(realValue);
}

void Lexer::makeToken(std::string name, TokenType type, TokenValue value, std::string strValue) {
    makeToken(name, type, value);
    token.setStrValue(strValue);
}

void Lexer::handleNum() {
    int intValue = 0;
    char c;
    while (input.get(c) && std::isdigit(c)){
        intValue *= 10;
        intValue += c - '0';
        buffer.push_back(c);
    }
    if(input.eof()){
        makeToken(buffer, TokenType::INTEGER , TokenValue::UNDEFINE , intValue);
        return;
    }
    if(c != '.'){
        input.unget();
        makeToken(buffer, TokenType::INTEGER , TokenValue::UNDEFINE , intValue);
        return;
    }
    else {
        double realValue = 0;
        buffer.push_back(c);

        while(input.get(c) && std::isdigit(c)){
            realValue += c - '0';
            realValue /= 10;
            buffer.push_back(c);
        }
        realValue += intValue;
        if (!input.eof()){
            input.unget();
        }
        makeToken(buffer, TokenType::REAL , TokenValue::UNDEFINE , realValue);
        return;
    }
}

void Lexer::handleIdentifier() {
    char c;
    while (input.get(c) && isalnum(c)){
        buffer.push_back(c);
    }
    if(!input.eof()){
        input.unget();
    }
    if(symbol.haveToken(buffer)){
        auto tokenTuple = symbol.findToken(buffer);
        makeToken(buffer, std::get<0>(tokenTuple), std::get<1>(tokenTuple));
        return;
    }
    else {
        symbol.addToken(buffer, TokenType::IDENTIFIER , TokenValue::VARIABLE);
        makeToken(buffer, TokenType::IDENTIFIER , TokenValue::VARIABLE);
        return;
    }
}

void Lexer::handleOther() {
    char c;
    input.get(c);
    buffer.push_back(c);

    if(c == '\n'){
        line++;
        buffer.clear();
        setNextToken();
        return;
    }
    if(c == '/'){
        if(input.get(c) && (c == '/' | '*')){
            input.unget();
            handleComment();
            setNextToken();
            return;
        }
    }


    if(input.get(c)){
        buffer.push_back(c);
        if(symbol.haveToken(buffer)){
            auto tokenTuple = symbol.findToken(buffer);
            makeToken(buffer, std::get<0>(tokenTuple), std::get<1>(tokenTuple));
            return;
        }
        buffer.pop_back();
        input.unget();
    }

    if(symbol.haveToken(buffer)){
        auto tokenTuple = symbol.findToken(buffer);
        makeToken(buffer, std::get<0>(tokenTuple), std::get<1>(tokenTuple));
        return;
    }
    else {
        error();
    }

}

void Lexer::handleComment() {
    char c, nextc;
    if(input.get(c) && input.get(nextc)){
        if(c == '/' && nextc == '/'){
            while(input.get(c) && c != '\n');
            if(!input.eof())input.unget();
            return;
        }else if (c == '/' && nextc == '*'){
            input.get(c);
            while(input.get(nextc)){
                if(c == '*' && nextc == '/'){
                    return;
                }
                if(nextc == '\n'){
                    line++;
                }
                c = nextc;
            }
            error();
        }
    }
}

void Lexer::setNextToken() {
    if(input.eof()){
        makeToken("END_OF_FILE", TokenType::END_OF_FILE , TokenValue::UNDEFINE);
    }
    char c;
    input.get(c);
    while (c == 32){
        input.get(c);
    }
    input.unget();

    if (std::isdigit(c)){
        handleNum();
    }
    else if (std::isalpha(c)){
        handleIdentifier();
    }
    else {
        handleOther();
    }
    buffer.clear();
}

Token Lexer::getNextToken() {
    setNextToken();
    Token res = token;
    return res;
}