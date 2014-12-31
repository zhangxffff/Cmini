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
    if (first.symbolType < second.symbolType) {
        return true;
    }
    else if (first.symbolType == SymbolType::NOTERMINAL) {
        return first.noTerminalType < second.noTerminalType;
    }
    else if (first.symbolType == SymbolType::TERMINAL) {
        return first.terminalType < second.terminalType;
    }
    return false;
}


// class Rule

Symbol const Rule::getRightFirst() const {
    if (right.empty()) {
        return {TerminalType::NONE};
    }
    return right.front();
}


// class Grammar

std::set<TerminalType> Grammar::getFirstFromSymbol(Symbol symbol) {
    std::set <TerminalType> firstSymbol;
    if (symbol.symbolType == SymbolType::TERMINAL) {
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

std::set<TerminalType> Grammar::getFollow(Symbol left) {
    std::set <TerminalType> follow;
    for (auto rule : grammar) {
        auto symbol = rule.getRight().begin();
        while (symbol != rule.getRight().end()) {
            if (*symbol == left) {
                symbol++;
                if (symbol != rule.getRight().end()) {
                    auto first = getFirstFromSymbol(*symbol);
                    follow.insert(first.begin(), first.end());
                } else if (!(left == rule.getLeft())) {
                    auto followSymbol = getFollow(rule.getLeft());
                    follow.insert(followSymbol.begin(), followSymbol.end());
                }
            }
            else {
                symbol++;
            }
        }
    }
    return follow;
}


// class Tree

void Tree::print(int &i) {
    if(child.empty())return;
    std::cout << std::setw(5) << i << ":";
    std::cout << std::setw(15) << symbol.noTerminalType << std::setw(15) << "Children:  ";
    for(auto tree : child){
        std::cout<< tree->symbol;
    }
    std::cout << std::endl;

    for(auto tree : child){
        i++;
        tree->print(i);
    }
}

void Tree::addChild(Symbol symbol) {
    auto treePtr = std::make_shared<Tree>(symbol);
    treePtr->parent = std::make_shared<Tree>(*this);
    child.push_back(treePtr);
}



//class Parser

Parser::Parser() : grammar(ruleList) {
    for(int i = 0; i < (int)NoTerminalType::NOTERMINAL_NUM; i++){
        for(int j = 0; j < (int)TerminalType::TERMINAL_NUM; j++){
            setAnalyTableEntity((NoTerminalType)i, (TerminalType)j, RuleType::NONE);
        }
    }
    setAnalyTable();
    tree = std::make_shared<Tree>(NoTerminalType::STMT);

    priority.insert({TerminalType::ASSIGN, 0});
    priority.insert({TerminalType::OR, 1});
    priority.insert({TerminalType::AND, 2});
    priority.insert({TerminalType::BITOR, 3});
    priority.insert({TerminalType::BITAND, 5});
    priority.insert({TerminalType::NOT_EAUAL, 6});
    priority.insert({TerminalType::EQUAL, 6});
    priority.insert({TerminalType::GREATER, 7});
    priority.insert({TerminalType::NOT_GREATER, 7});
    priority.insert({TerminalType::LESS, 7});
    priority.insert({TerminalType::NOT_LESS, 7});
    priority.insert({TerminalType::LSHIFT, 8});
    priority.insert({TerminalType::LSHIFT, 8});
    priority.insert({TerminalType::ADD, 9});
    priority.insert({TerminalType::SUB, 9});
    priority.insert({TerminalType::MUL, 10});
    priority.insert({TerminalType::DIV, 10});
    priority.insert({TerminalType::NOT, 11});
    priority.insert({TerminalType::BITNOT, 11});
    priority.insert({TerminalType::LPAR, 14});
    priority.insert({TerminalType::RPAR, 14});
    priority.insert({TerminalType::ID, 15});
    priority.insert({TerminalType::INC, 15});
    priority.insert({TerminalType::DEC, 15});
    priority.insert({TerminalType::INTNUM, 15});
    priority.insert({TerminalType::DOUBLENUM, 15});

}

//  初始化分析表
void Parser::setAnalyTable() {
    for(auto rule : grammar.getGrammar()){
        for(auto terminal : grammar.getFirstFromSymbol(rule.getRightFirst())){
            if(terminal == TerminalType::NONE){
                //TODO FOLLOW
                auto follow = grammar.getFollow(rule.getLeft());
                for(auto terminal : follow){
                    setAnalyTableEntity(rule.getLeft().noTerminalType, terminal, rule.getRuleType());
                }
            }
            else {
                setAnalyTableEntity(rule.getLeft().noTerminalType, terminal, rule.getRuleType());
            }
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
                std::cout << "INPUT ERROR:  " << lexer.getToken().getValue() << std::endl;
                exit(0);
            }
        }
        else if(symbol.noTerminalType == NoTerminalType::EXPR){
            exprList.clear();
            token = lexer.getToken();
            while(token.getType() == TokenType::IDENTIFIER ||
                    token.getType() == TokenType::OPREATOR ||
                    token.getType() == TokenType::INTEGER  ||
                    token.getType() == TokenType::REAL){
                token = lexer.getNextToken();
                exprList.push_back(std::make_shared<Tree>(token));
                token = lexer.getToken();
            }
            handleExpr();
        }
        else {
            //TODO
            token = lexer.getToken();
            auto ruleType = checkAnalyTable(symbol.noTerminalType, token.getValue());
            if(ruleType != RuleType::NONE){
                analyStack.pop();
                Rule rule = grammar.getRule(ruleType);
                if(rule.getRight().empty()){
                    continue;
                }
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
    auto treePtr = analyStack.top();
    do {
        decltype(exprList.begin()) end = endExpr(exprList.begin());
        decltype(end) begin = beginExpr(end);
        if(end == begin){
            switch ((*end)->getSymbol().terminalType){
                case TerminalType::INTNUM:
                case TerminalType::DOUBLENUM:{
                    auto ptr = *begin;
                    (*begin) = std::make_shared<Tree>(NoTerminalType::EXPR_NUM);
                    (*begin)->addChild(ptr);
                    break;
                };
                case TerminalType::ID: {
                    auto ptr = *begin;
                    (*begin) = std::make_shared<Tree>(NoTerminalType::EXPR_ID);
                    (*begin)->addChild(ptr);
                    break;
                };
                case TerminalType::INC:{
                    auto ptr = *begin;
                    auto left = std::prev(begin, 1);
                    *begin = std::make_shared<Tree>(NoTerminalType::EXPR_INC);
                    (*begin)->addChild(*left);
                    (*begin)->addChild(ptr);
                    exprList.erase(left);
                    break;
                };
                case TerminalType::ADD:
                    handleOperator2(begin, NoTerminalType::EXPR_ADD);
                    break;
                case TerminalType::SUB:
                    handleOperator2(begin, NoTerminalType::EXPR_SUB);
                    break;
                case TerminalType::MUL:
                    handleOperator2(begin, NoTerminalType::EXPR_MUL);
                    break;
                case TerminalType::DIV:
                    handleOperator2(begin, NoTerminalType::EXPR_DIV);
                    break;
                case TerminalType::MOD:
                    handleOperator2(begin, NoTerminalType::EXPR_MOD);
                    break;
                case TerminalType::GREATER:
                    handleOperator2(begin, NoTerminalType::EXPR_GREATER);
                    break;
                case TerminalType::LESS:
                    handleOperator2(begin, NoTerminalType::EXPR_LESS);
                    break;
                case TerminalType::EQUAL:
                    handleOperator2(begin, NoTerminalType::EXPR_EQUAL);
                    break;
                default:
                    break;
            }
        }
        else if(std::distance(begin, end) == 1){
            if((*end)->getSymbol().terminalType == TerminalType::INC){
                auto ptr = *end;
                auto left = begin;
                *end = std::make_shared<Tree>(NoTerminalType::EXPR_INC);
                (*end)->addChild(*left);
                (*end)->addChild(ptr);
                exprList.erase(left);
                break;
            }
        }
    }while(exprList.size() > 1);
    treePtr->addChild(exprList.front());
    analyStack.pop();
    return;
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

void Parser::printTree() {
    int i = 0;
    tree->print(i);
}

Comp Parser::compare(Symbol first, Symbol second) {

    if(priority[first.terminalType] == priority[second.terminalType]){
        if(first.terminalType == TerminalType::ADD ||
                first.terminalType == TerminalType::SUB ||
                first.terminalType == TerminalType::MUL ||
                first.terminalType == TerminalType::DIV){
            return Comp::HIGHER;
        }
        return Comp::EQUAL;
    }else if(priority[first.terminalType] > priority[second.terminalType]){
        return Comp::HIGHER;
    }else{
        return Comp::LOWER;
    }
}


//跳过非终结符
decltype(Parser::exprList.begin()) Parser::endExpr(decltype(exprList.begin()) start) {
    while((*start)->getSymbol().symbolType == SymbolType::NOTERMINAL && start != exprList.end()){
        start++;
    }
    auto next = start;
    next++;
    while(next != exprList.end()){
        if((*next)->getSymbol().symbolType == SymbolType::TERMINAL) {
            if (compare((*start)->getSymbol(), (*next)->getSymbol()) == Comp::HIGHER) {
                return start;
            }
            else{
                start = next;
            }
        }
        next++;
    }
    return start;
}



decltype(Parser::exprList.begin()) Parser::beginExpr(decltype(exprList.begin()) start) {
    while((*start)->getSymbol().symbolType == SymbolType::NOTERMINAL && start != exprList.begin()){
        start--;
    }
    auto previous = start;
    while(previous != exprList.begin()){
        previous--;
        if((*previous)->getSymbol().symbolType == SymbolType::TERMINAL) {
            if (compare((*previous)->getSymbol(), (*start)->getSymbol()) == Comp::LOWER) {
                return start;
            }
            else{
                start = previous;
            }
        }
    }
    return start;
}


void Tree::addChild(std::shared_ptr<Tree> treePtr) {
    child.push_back(treePtr);
}

void Parser::handleOperator2(decltype(exprList.begin()) begin, NoTerminalType noTerminalType) {
    auto ptr = *begin;
    auto left = std::prev(begin, 1);
    auto right = std::next(begin, 1);
    (*begin) = std::make_shared<Tree>(noTerminalType);
    (*begin)->addChild(*(left));
    (*begin)->addChild(ptr);
    (*begin)->addChild(*(right));
    exprList.erase(left);
    exprList.erase(right);
}