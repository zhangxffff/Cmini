#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include "parser.h"

/*  上下文无关文法的扩充文法
 *  原来的开始符号是STMTBLOCK，即一个程序块
 *
 *
 *  STMTBLOCK部分
 *
 *  STMT ==> {MULTISTMT}
 *  MULTISTMT ==> STMT ; MULTISTMT
 *  MULTISTMT ==> STMT
 *
 *  STMT部分
 *
 *  STMT ==> TYPE IDLIST;
 *  IDLIST ==> ID , IDLIST
 *  IDLIST ==> ID
 *  TYPE ==> INT
 *  TYPE ==> DOUBLE
 *
 *  STMT ==> ID = EXPR;
 *
 *  STMT ==> if ( EXPR ) STMT ELSE STMT
 *
 *  STMT ==> WHILE ( EXPR ) STMT
 *
 *  STMT ==> FOR ( EXPR ; EXPR; EXPR ) STMT
 *
 *
 *  EXPR部分
 *
 *  利用算符优先得到语法树
 */

static const std::vector<Rule> ruleList = {
        //STMT_BLOCK,    STMT ==> {MULTISTMT}
        {
                {RuleType::STMT_BLOCK},
                {NoTerminalType::STMT},
                {{TerminalType::LBRACE}, {NoTerminalType::MULTISTMT}, {TerminalType::RBRACE}}
        },
        //MULITISTMT,     MULTISTMT ==> STMT ; MULTISTMT
        {
                {RuleType::MULITISTMT},
                {NoTerminalType::MULTISTMT},
                {{NoTerminalType::STMT}, {TerminalType::SEMI}, {NoTerminalType::MULTISTMT}}
        },
        //MULTISTMT_END,    MULTISTMT ==> STMT
        {
                {RuleType::MULTISTMT_END},
                {NoTerminalType::MULTISTMT},
                {{NoTerminalType::STMT}}
        },
        //STMT_ASSIGN,     STMT ==> ID = EXPR;
        {
                {RuleType::STMT_ASSIGN},
                {NoTerminalType::STMT},
                {{TerminalType::ID}, {TerminalType::EQUAL}, NoTerminalType::EXPR}
        },
        //STMT_IF,         STMT ==> if ( EXPR ) STMT ELSE STMT
        {
                {RuleType::STMT_IF},
                {NoTerminalType::STMT},
                {{TerminalType::IF}, {TerminalType::LPAR}, {NoTerminalType::EXPR}, {TerminalType::RPAR}, {NoTerminalType::STMT}, {TerminalType::ELSE}, {NoTerminalType::STMT}}
        },
        //STMT_WHILE,      STMT ==> WHILE ( EXPR ) STMT
        {
                {RuleType::STMT_WHILE},
                {NoTerminalType::STMT},
                {{TerminalType::WHILE}, {TerminalType::LPAR}, {NoTerminalType::EXPR}, {TerminalType::RPAR}, {NoTerminalType::STMT}}
        },
        //STMT_FOR,        STMT ==> FOR ( EXPR ; EXPR; EXPR ) STMT
        {
                {RuleType::STMT_FOR},
                {NoTerminalType::STMT},
                {{TerminalType::FOR}, {TerminalType::LPAR}, {NoTerminalType::EXPR}, {TerminalType::SEMI}, {NoTerminalType::EXPR}, {TerminalType::SEMI}, {NoTerminalType::EXPR}, {TerminalType::RPAR}, {NoTerminalType::STMT}}
        },
        //STMT_DECL,       STMT ==> TYPE IDLIST;
        {
                {RuleType::STMT_DECL},
                {NoTerminalType::STMT},
                {{NoTerminalType::TYPE}, {NoTerminalType::IDLIST}, {TerminalType::SEMI}}
        },
        //TYPE_INT,        TYPE ==> INT
        {
                {RuleType::TYPE_INT},
                {NoTerminalType::TYPE},
                {{TerminalType::INT}}
        },
        //TYPE_DOUBLE,     TYPE ==> DOUBLE
        {
                {RuleType::TYPE_DOUBLE},
                {NoTerminalType::TYPE},
                {{TerminalType::DOUBLE}}
        },
        //IDLIST,          IDLIST ==> ID , IDLIST
        {
                {RuleType::IDLIST},
                {NoTerminalType::IDLIST},
                {{TerminalType::ID}, {TerminalType::COMMA}, {NoTerminalType::IDLIST}}
        },
        //IDLIST_END,      IDLIST ==> ID
        {
                {RuleType::IDLIST_END},
                {NoTerminalType::IDLIST},
                {{TerminalType::ID}}
        }
};

#endif