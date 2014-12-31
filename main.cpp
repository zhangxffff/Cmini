#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "test.h"
#include <sstream>
#include "generate.h"

int main(int argc, char *argv[]) {


    std::string input = "/home/hulianthus/Programs/Projects/CLionProjects/Complier/input.txt";
    std::string output = "/home/hulianthus/Programs/Projects/CLionProjects/Complier/output.asm";
    Lexer lexer(input);
    Parser parser;

    parser.analysis(lexer);

    Generate generate(output);

    generate.genAll(*parser.getTree());

    return 0;
}