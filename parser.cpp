#include <iomanip>
#include "parser.h"
#include "grammar.h"
#include "test.h"
#include "lexer.h"

// class Symbol
bool operator==(const Symbol &first, const Symbol &second) {
    return first.equal(second);
}

bool Symbol::equal(const Symbol &symbol) const {
    if (symbolType == symbol.symbolType) {
        switch (symbolType) {
            case SymbolType::TERMINAL:
                return symbol.terminalType == terminalType;
            case SymbolType::NOTERMINAL:
                return symbol.noTerminalType == noTerminalType;
        }
    }
    else {
        return false;
    }
}

bool operator<(const Symbol &first, const Symbol &second) {
    if(first.symbolType < second.symbolType){
        return true;
    }
    else if(first.symbolType == SymbolType::NOTERMINAL){
        return first.noTerminalType < second.noTerminalType;
    }
    else if(first.symbolType == SymbolType::TERMINAL){
        return first.terminalType < second.terminalType;
    }
    return false;
}

// class Rule

Symbol const &Rule::getRightFirst() const {
    return right.front();
}




// class Grammar

std::set<TerminalType> Grammar::getFirstFromSymbol(Symbol symbol) {
    std::set<TerminalType> firstSymbol;
    if(symbol.symbolType == SymbolType::TERMINAL){
        firstSymbol.insert(symbol.terminalType);
    }
    else {
        for (auto rule : grammar) {
            if (rule.getLeft() == symbol) {
                if (rule.getRightFirst().symbolType == SymbolType::TERMINAL) {
                    firstSymbol.insert(rule.getRightFirst().terminalType);
                }
                else {
                    auto tempSymbol = getFirstFromSymbol(rule.getRightFirst());
                    firstSymbol.insert(tempSymbol.begin(), tempSymbol.end());
                }
            }
        }
    }
    return firstSymbol;

}





Parser::Parser() : grammar(ruleList) {
    for(int i = 0; i < (int)NoTerminalType::NOTERMINAL_NUM; i++){
        for(int j = 0; j < (int)TerminalType::TERMINAL_NUM; j++){
            setAnalyTableEntity((NoTerminalType)i, (TerminalType)j, RuleType::NONE);
        }
    }
    setAnalyTable();
    tree = std::make_shared<Tree>(NoTerminalType::STMT);
}

//  初始化分析表
void Parser::setAnalyTable() {
    for(auto rule : grammar.getGrammar()){
        for(auto terminal : grammar.getFirstFromSymbol(rule.getRightFirst())){
            setAnalyTableEntity(rule.getLeft().noTerminalType, terminal, rule.getRuleType());
        }
    }
}

void Parser::analysis(Lexer& lexer) {
    Token token;
    analyStack.push(tree);
    do{
        auto treePtr = analyStack.top();
        auto symbol = treePtr->getSymbol();
        if(symbol.symbolType == SymbolType::TERMINAL) {
            token = lexer.getNextToken();
            if(symbol.terminalType == token.getValue()){
                treePtr->setTokenPtr(std::make_shared<Token>(token));
                analyStack.pop();
            }
            else {
                //TODO ERROR
            }
        }
        else if(symbol.noTerminalType == NoTerminalType::EXPR){
            handleExpr();
        }
        else {
            //TODO
            token = lexer.getToken();
            auto ruleType = checkAnalyTable(symbol.noTerminalType, token.getValue());
            if(ruleType != RuleType::NONE){
                analyStack.pop();
                Rule rule = grammar.getRule(ruleType);
                for(auto ruleSymbol : rule.getRight()){
                    treePtr->addChild(ruleSymbol);
                }
                auto childTree = treePtr->getChild().end();
                do{
                    childTree--;
                    analyStack.push(*childTree);
                }while(childTree != treePtr->getChild().begin());

            }
        }
    }while((!analyStack.empty()));
}

void Parser::handleExpr() {
    //TODO EXPR
}

void Parser::printAnalyTable() {
    for(int i = 0; i < (int)NoTerminalType::NOTERMINAL_NUM; i++){
        for(int j = 0; j < (int)TerminalType::TERMINAL_NUM; j++){
            if(analyTable[i][j] != RuleType::NONE) {
                std::cout << std::setw(10) << (NoTerminalType) i << std::setw(10) << (TerminalType) j << std::setw(20) << analyTable[i][j] << std::endl;
            }
        }
    }
}

void Tree::print() {
    std::cout << std::setw(10) << symbol << ":";
    for(auto tree : child){
        std::cout << std::setw(10) << tree->symbol;
    }
    std::cout << std::endl;
    for(auto tree : child){
        tree->print();
    }
}

void Parser::printTree() {
    tree->print();
}

void Tree::addChild(Symbol symbol) {
    auto treePtr = std::make_shared<Tree>(symbol);
    treePtr->parent = std::make_shared<Tree>(*this);
    child.push_back(treePtr);
}