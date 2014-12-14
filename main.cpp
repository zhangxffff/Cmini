#include <iostream>
#include <string>
#include <iomanip>
#include "lexer.h"
#include "test.h"
#include "grammar.h"


int main() {

    std::string filename = "/home/hulianthus/Programs/Projects/CLionProjects/Complier/input.txt";
    Lexer lexer(filename);
    Parser parser;
    parser.analysis(lexer);

    parser.printTree();

    return 0;
}