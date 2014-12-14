#include "test.h"

std::ostream &operator<<(std::ostream &out, const NoTerminalType &noTerminalTypeconst) {
    switch (noTerminalTypeconst) {
        case NoTerminalType::MULTISTMT:
            return out << "MULTISTMT";
        case NoTerminalType::STMT:
            return out << "STMT";
        case NoTerminalType::TYPE:
            return out << "TYPE";
        case NoTerminalType::IDLIST:
            return out << "IDLIST";
        case NoTerminalType::EXPR:
            return out << "EXPR";
        default:
            return out;
    }
}

std::ostream &operator<<(std::ostream &out, const TerminalType &terminalTypeconst) {
    switch (terminalTypeconst) {
        case TerminalType::INT:
            return out << "int";
        case TerminalType::DOUBLE:
            return out << "double";
            //分支与循环
        case TerminalType::IF:
            return out << "if";
        case TerminalType::ELSE:
            return out << "else";
        case TerminalType::FOR:
            return out << "for";
        case TerminalType::WHILE:
            return out << "while";

            //算术运算
        case TerminalType::ADD:
            return out << "+";
        case TerminalType::SUB:
            return out << "-";
        case TerminalType::MUL:
            return out << "*";
        case TerminalType::DIV:
            return out << "/";
        case TerminalType::ASSIGN:
            return out << "=";

            //逻辑运算
        case TerminalType::AND:
            return out << "AND";
        case TerminalType::OR:
            return out << "OR";
        case TerminalType::NOT:
            return out << "NOT";


            //位运算
        case TerminalType::LSHIFT:
            return out << "LSHIFT";
        case TerminalType::RSHIFT:
            return out << "RSHIFT";
        case TerminalType::BITNOT:
            return out << "BITNOT";
        case TerminalType::BITAND:
            return out << "BITAND";
        case TerminalType::BITOR:
            return out << "BITOR";

            //比较
        case TerminalType::EQUAL:
            return out << "==";
        case TerminalType::NOT_EAUAL:
            return out << "!=";
        case TerminalType::GREATER:
            return out << "GREATER";
        case TerminalType::LESS:
            return out << "LESS";
        case TerminalType::NOT_GREATER:
            return out << "NOT_GREATER";
        case TerminalType::NOT_LESS:
            return out << "NOT_LESS";

            //括号与逗号
        case TerminalType::LPAR:
            return out << "(";
        case TerminalType::RPAR:
            return out << ")";
        case TerminalType::LBRACE:
            return out << "{";
        case TerminalType::RBRACE:
            return out << "}";
        case TerminalType::COMMA:
            return out << ",";
        case TerminalType::SEMI:
            return out << ";";

        case TerminalType::ID:
            return out << "ID";
        case TerminalType::INTNUM:
            return out << "INTNUM";
        case TerminalType::DOUBLENUM:
            return out << "DOUBLENUM";
        case TerminalType::UNDEFINE:
            return out << "UNDEFINE";
        default:
            return out;
    }
}

std::ostream &operator<<(std::ostream &out, const Symbol &symbol) {
    out << " ";
    switch (symbol.symbolType) {
        case SymbolType::TERMINAL:
            return out << symbol.terminalType;
        case SymbolType::NOTERMINAL:
            return out << symbol.noTerminalType;
    }
}

std::ostream &operator<<(std::ostream &out, const Rule &rule) {
    out << rule.getLeft() << " ==>";
    auto &right = rule.getRight();
    for (auto symbol : right) {
        out << symbol;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const Grammar &grammarconst) {
    for (auto rule : grammarconst.getGrammar()) {
        out << rule << std::endl;
    }
    return out;
}


std::ostream &operator<<(std::ostream &out, const RuleType &ruleTypeconst) {
    switch (ruleTypeconst) {
        case RuleType::STMT_BLOCK:
            out << "STMT_BLOCK";
            break;
        case RuleType::MULITISTMT:
            out << "MULITISTMT";
            break;
        case RuleType::MULTISTMT_END:
            out << "MULTISTMT_END";
            break;
        case RuleType::STMT_ASSIGN:
            out << "STMT_ASSIGN";
            break;
        case RuleType::STMT_IF:
            out << "STMT_IF";
            break;
        case RuleType::STMT_WHILE:
            out << "STMT_WHILE";
            break;
        case RuleType::STMT_FOR:
            out << "STMT_FOR";
            break;
        case RuleType::STMT_DECL:
            out << "STMT_DECL";
            break;
        case RuleType::TYPE_INT:
            out << "TYPE_INT";
            break;
        case RuleType::TYPE_DOUBLE:
            out << "TYPE_DOUBLE";
            break;
        case RuleType::IDLIST:
            out << "IDLIST";
            break;
        case RuleType::IDLIST_END:
            out << "IDLIST_END";
            break;
        case RuleType::NONE:
            out << "NONE";
    }
    return out;
}
