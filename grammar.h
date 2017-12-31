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
        //MULITISTMT,     MULTISTMT ==> STMT MULTISTMT
        {
                {RuleType::MULITISTMT},
                {NoTerminalType::MULTISTMT},
                {{NoTerminalType::STMT}, {NoTerminalType::MULTISTMT}}
        },
        //MULTISTMT_END,    MULTISTMT ==> VOID
        {
                {RuleType::MULTISTMT_END},
                {NoTerminalType::MULTISTMT},
                {}
        },

        //STMT_OUTPUT,     STMT_OUTPUT ==> OUTPUT ( ID );
        {
                {RuleType::STMT_OUTPUT},
                {NoTerminalType::STMT},
                {{TerminalType::OUTPUT}, {TerminalType::LPAR}, {TerminalType::ID }, {TerminalType::RPAR}, {TerminalType::SEMI}}
        },
        //STMT_INPUT,     STMT ==> INPUT ( ID );
        {
                {RuleType::STMT_INPUT},
                {NoTerminalType::STMT},
                {{TerminalType::INPUT}, {TerminalType::LPAR}, {TerminalType::ID}, {TerminalType::RPAR}, {TerminalType::SEMI}}
        },
        //STMT_ASSIGN,     STMT ==> ID = EXPR;
        {
                {RuleType::STMT_ASSIGN},
                {NoTerminalType::STMT},
                {{TerminalType::ID}, {TerminalType::ASSIGN}, {NoTerminalType::EXPR}, {TerminalType::SEMI}}
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
        //IDLIST_START,    IDLIST ==> ID COMMA_ID
        {
                {RuleType::IDLIST},
                {NoTerminalType::IDLIST},
                {{TerminalType::ID}, {NoTerminalType::COMMA_ID}}
        },
        //IDLIST_END,      COMMA_ID ==> , ID COMMA_ID
        {
                {RuleType::IDLIST_END},
                {NoTerminalType::COMMA_ID},
                {{TerminalType::COMMA}, {TerminalType::ID}, {NoTerminalType::COMMA_ID }}
        },
        //IDLIST_VOID,     COMMA_ID ==> VOID
        {
                {RuleType::IDLIST_VOID},
                {NoTerminalType::COMMA_ID},
                {}
        },

};

#endif