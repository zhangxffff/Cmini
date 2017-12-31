#ifndef TEST_H
#define TEST_H
#include <iostream>
#include "parser.h"

std::ostream & operator<<(std::ostream &out, const NoTerminalType &noTerminalTypeconst);

std::ostream & operator<<(std::ostream &out, const TerminalType &terminalTypeconst);

std::ostream & operator<<(std::ostream &out, const Symbol &symbol);

std::ostream & operator<<(std::ostream &out, const Rule &rule);

std::ostream & operator<<(std::ostream& out, const Grammar& grammarconst);

std::ostream & operator<<(std::ostream& out, const RuleType& ruleTypeconst);

#endif