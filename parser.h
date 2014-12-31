#ifndef PARSERLR_H
#define PARSERLR_H

#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <stack>
#include <memory>
#include <iomanip>
#include "token.h"
#include "lexer.h"

typedef TokenValue TerminalType;

enum class Comp{
    HIGHER,
    LOWER,
    EQUAL
};

enum class SymbolType {
    TERMINAL,
    NOTERMINAL
};

enum class NoTerminalType {
    STMT,
    MULTISTMT,


    TYPE,
    IDLIST,
    COMMA_ID,
    EXPR,

    EXPR_ID,
    EXPR_NUM,

    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_MOD,

    EXPR_GREATER,
    EXPR_LESS,
    EXPR_EQUAL,

    EXPR_INC,
    EXPR_DEC,


    EXPR_PAR,

    NOTERMINAL_NUM
};

enum class RuleType {
    NONE,

    STMT_BLOCK,             //    STMT ==> {MULTISTMT}
    MULITISTMT,             //    MULTISTMT ==> STMT ; MULTISTMT
    MULTISTMT_END,          //    MULTISTMT ==> STMT
    STMT_ASSIGN,            //    STMT ==> ID = EXPR;
    STMT_IF,                //    STMT ==> if ( EXPR ) STMT ELSE STMT
    STMT_WHILE,             //    STMT ==> WHILE ( EXPR ) STMT
    STMT_FOR,               //    STMT ==> FOR ( EXPR ; EXPR; EXPR ) STMT
    STMT_DECL,              //    STMT ==> TYPE IDLIST;
    STMT_OUTPUT,
    STMT_INPUT,

    TYPE_INT,               //    TYPE ==> INT
    TYPE_DOUBLE,            //    TYPE ==> DOUBLE

    IDLIST,                 //    IDLIST ==> ID , IDLIST
    IDLIST_END,             //    IDLIST ==> ID
    IDLIST_VOID

};


struct Symbol {
    SymbolType symbolType;
    NoTerminalType noTerminalType;
    TerminalType terminalType;

    Symbol() {
    }

    Symbol(NoTerminalType const &noTerminalType) : noTerminalType(noTerminalType) {
        symbolType = SymbolType::NOTERMINAL;
    }

    Symbol(TerminalType const &terminalType) : terminalType(terminalType) {
        symbolType = SymbolType::TERMINAL;
    }

    bool equal(const Symbol &symbol) const;

};


bool operator == (const Symbol &first, const Symbol &second);

bool operator < (const Symbol &first, const Symbol &second);




//产生式
class Rule {
private:
    RuleType ruleType;
    Symbol left;
    std::list<Symbol> right;

public:


    Rule() {
    }

    Rule(RuleType const &ruleType, Symbol const &left, std::list<Symbol> const &right)
            : ruleType(ruleType), left(left), right(right) {
    }

    void setRuleType(RuleType const &ruleType) {
        Rule::ruleType = ruleType;
    }

    void setLeft(Symbol const &left) {
        Rule::left = left;
    }

    void setRight(std::list<Symbol> const &right) {
        Rule::right = right;
    }

    RuleType const &getRuleType() const {
        return ruleType;
    }

    Symbol const &getLeft() const {
        return left;
    }

    std::list<Symbol> const &getRight() const {
        return right;
    }

    Symbol const getRightFirst() const;

};


//上下文无关文法和扩充文法
class Grammar {
private:
    std::vector<Rule> grammar;
    std::map<NoTerminalType, std::set<TerminalType>> first;

public:
    Grammar(std::vector<Rule> const &grammar) : grammar(grammar) {
    }

    std::set<TerminalType> getFirstFromSymbol(Symbol symbol);

    std::set<TerminalType> getFollow(Symbol left);

    std::map<NoTerminalType, std::set<TerminalType>> const &getFirst() const {
        return first;
    }

    std::vector<Rule> const &getGrammar() const {
        return grammar;
    }

    Rule getRule(RuleType ruleType){
        for(auto rule : grammar){
            if(rule.getRuleType() == ruleType){
                return rule;
            }
        }
        return {};
    }

};


class Tree{
private:
    Symbol symbol;
    std::shared_ptr<Token> tokenPtr;
    std::shared_ptr<Tree> parent;
    std::vector<std::shared_ptr<Tree>> child;
public:

    Tree() {
    }

    Tree(Token& token){
        tokenPtr = std::make_shared<Token>(token);
        symbol = Symbol(token.getValue());
    }

    Tree(Symbol const &symbol) : symbol(symbol) {
    }

    Symbol const &getSymbol() const {
        return symbol;
    }

    std::shared_ptr<Tree> const &getParent() const {
        return parent;
    }


    std::vector<std::shared_ptr<Tree>> const &getChild() const {
        return child;
    }

    void setSymbol(Symbol const &symbol) {
        Tree::symbol = symbol;
    }

    void setParent(std::shared_ptr<Tree> const &parent) {
        Tree::parent = parent;
    }


    void setChild(std::vector<std::shared_ptr<Tree>> const &child) {
        Tree::child = child;
    }

    std::shared_ptr<Token> const &getTokenPtr() const {
        return tokenPtr;
    }

    void setTokenPtr(std::shared_ptr<Token> const &tokenPtr) {
        Tree::tokenPtr = tokenPtr;
    }

    void addChild(Symbol symbol);

    void addChild(std::shared_ptr<Tree> treePtr);

    void print(int&);
};



class Parser{
private:
    Grammar grammar;
    RuleType analyTable[(int)NoTerminalType::NOTERMINAL_NUM][(int)TerminalType::TERMINAL_NUM];
    std::stack<std::shared_ptr<Tree>> analyStack;
    std::shared_ptr<Tree> tree;
    std::list<std::shared_ptr<Tree>> exprList;
    std::map<TerminalType, int> priority;

public:
    Parser();

    std::shared_ptr<Tree> const &getTree() const {
        return tree;
    }

    void setAnalyTable();

    Comp compare(Symbol, Symbol);

    decltype(exprList.begin()) endExpr(decltype(exprList.begin()));

    decltype(exprList.begin()) beginExpr(decltype(exprList.begin()));

    void handleOperator2(decltype(exprList.begin()) begin, NoTerminalType noTerminalType);

    void setAnalyTableEntity(NoTerminalType noTerminalType, TerminalType terminalType, RuleType ruleType) {
        analyTable[(int)noTerminalType][(int)terminalType] = ruleType;
    }


    RuleType checkAnalyTable(NoTerminalType noTerminalType, TerminalType terminalType){
        return analyTable[(int)noTerminalType][(int)terminalType];
    }

    void analysis(Lexer&);

    void handleExpr();

    void printAnalyTable();

    void printTree();
};


#endif