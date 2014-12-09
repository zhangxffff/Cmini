#include <iostream>
#include <string>
#include <iomanip>
#include "lexer.h"

int main()
{
    std::string fileName = "/home/hulianthus/Programs/Projects/CLionProjects/Complier/input.txt";
    Lexer lexer(fileName);
    std::cout << std::setw(10) << "Line" << std::setw(10) << "Name" << std::setw(10) << "Type" << std::setw(10) << "Value" << std::endl;
    while(lexer.getNextToken().getType() != TokenType::END_OF_FILE){
        std::cout << std::setw(10) << lexer.getToken().getLine() << std::setw(10) << lexer.getToken().getName() << std::endl;
    }

}