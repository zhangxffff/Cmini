#include "parser.h"
#include <fstream>
#include <sstream>
#include <string>

class Generate{
private:
    std::ofstream outfile;
    std::stringstream data;
    std::stringstream text;
    int varNum;
    int labelNum;
    std::map<std::string, std::string> varTable;
    std::set<std::string> reg;

    std::string newVar(){
        std::string var = "var" + std::to_string(varNum++);
        data << var << "\tdd\t0\n";
        return var;
    }

    std::string newLabel(){
        return "L" + std::to_string(labelNum++);
    }

    std::string movId(std::string src, std::string dst){
        if(src == dst)return "";
        if(reg.find(src) == reg.end() && reg.find(dst) == reg.end()){
            return std::string() + "\tmov eax, [" + src + "]\n\tmov [" + dst + "], eax\n";
        }
        if(reg.find(src) == reg.end()){
            src = std::string("[") + src + "]";
        }
        if(reg.find(dst) == reg.end()){
            dst = std::string("[") + dst + "]";
        }
        return std::string() + "\tmov " + dst + ", " + src + "\n";
    }

    std::string getReg(){
        std::string randomReg = *reg.begin();
        reg.erase(reg.begin());
        return randomReg;
    }
    std::string getReg(std::string regStr){
        reg.erase(reg.find(regStr));
        return regStr;
    }
    void returnReg(std::string regStr){
        reg.insert(regStr);
    }

public:
    Generate(std::string filename){
        varNum = labelNum = 0;
        reg = {"eax", "ebx", "ecx", "edx"};

        outfile.open(filename);
        data << "section\t.data\n" <<
                "num_printed\tdd\t8\n" <<
                "str_print_ln\tdb\t10, 0\n" <<
                "str_print_num\tdb\t\"%d\", 9, 9, 0\n" <<
                "str_input_num\tdb\t\"%d\", 0\n\n\n";

        text << "\nsection\t.text\n";

        text << "print_num:\n" <<
                "\tpush dword eax\n" <<
                "\tpush dword str_print_num\n" <<
                "\tcall printf\n" <<
                "\tadd esp, 8\n" <<
                "\tdec dword [num_printed]\n" <<
                "\tjnz R\n" <<
                "\tmov dword [num_printed], 8\n" <<
                "\tpush dword str_print_ln\n" <<
                "\tcall printf\n" <<
                "\tadd esp, 4\n" <<
                "R:\tret\n\n\n";

        text << "input_num:\n" <<
                "\tpush dword eax\n" <<
                "\tpush dword str_input_num\n" <<
                "\tcall scanf\n" <<
                "\tadd esp, 8\n" <<
                "\tret\n\n\n";


        text << "\tglobal main\n" <<
                "main:\n";
    }

    void genAll(Tree &tree){
        text << handleAll(tree);
        outfile << "extern printf\n" << "extern scanf\n\n\n";

        outfile << data.str();

        outfile << std::endl << std::endl;

        outfile << text.str();

        outfile << "\tpush dword str_print_ln\n\tcall printf\n\tadd esp, 4\n";
    }

    std::string handleAll(Tree &tree){
        return handleStmt(tree);
    }

    std::string handleStmt(Tree &tree){
        if(tree.getChild()[0]->getSymbol().symbolType == SymbolType::TERMINAL) {
            switch (tree.getChild()[0]->getSymbol().terminalType) {
                case TerminalType::OUTPUT:
                    return handleOutput(tree);
                case TerminalType::ID:
                    return handleAssign(tree);
                case TerminalType::LBRACE:
                    return handleMultistmt(*tree.getChild()[1]);
                case TerminalType::INPUT:
                    return handleInput(tree);
                case TerminalType::WHILE:
                    return handleWhile(tree);
                case TerminalType::IF:
                    return handleIf(tree);
                default:
                    break;
            }
        }
        else{
            switch (tree.getChild()[0]->getSymbol().noTerminalType){
                case NoTerminalType::TYPE:
                    handleDecl(tree);
                    break;
                default:
                    break;
            }
        }
        return "";
    }

    std::string handleAssign(Tree &tree){
        std::string varAsm = varTable[tree.getChild()[0]->getTokenPtr()->getName()];
        return handleExpr(*tree.getChild()[2], varAsm);
    }

    std::string handleMultistmt(Tree &tree){
        if(!tree.getChild().empty()){
            return handleStmt(*tree.getChild()[0]) + "\n" + handleMultistmt(*tree.getChild()[1]);
        }
        return "\n";
    }

    void handleDecl(Tree &tree){
        handleIdlist(*tree.getChild()[1]);
    }

    void handleIdlist(Tree &tree){
        varTable[tree.getChild()[0]->getTokenPtr()->getName()] = newVar();
        handleCommaId(*tree.getChild()[1]);
    }

    void handleCommaId(Tree &tree){
        if(tree.getChild().empty()){
            return;
        }
        else{
            varTable[tree.getChild()[1]->getTokenPtr()->getName()] = newVar();
            handleCommaId(*tree.getChild()[2]);
        }
    }

    std::string handleOutput(Tree &tree){
        std::string varName = tree.getChild()[2]->getTokenPtr()->getName();
        std::string varAsm = varTable[varName];
        return "\tmov eax, [" + varAsm + "]\n\tcall print_num\n\n";
    }

    std::string handleInput(Tree &tree){
        std::string varAsm = varTable[tree.getChild()[2]->getTokenPtr()->getName()];
        return "\tmov eax, " + varAsm + "\n\tcall input_num\n\n";
    }

    std::string handleWhile(Tree &tree){
        std::string str;
        std::string var = newVar();
        std::string labelStart = newLabel();
        std::string labelEnd = newLabel();


        str += labelStart + ":\n";
        str += handleExpr(*tree.getChild()[2], var);
        str += "\tmov eax, dword [" + var + "]\n";
        str += "\tcmp eax, 0\n";
        str += "\tjz " + labelEnd + "\n";
        str += handleStmt(*tree.getChild()[4]);
        str += "\tjmp " + labelStart + "\n";
        str += labelEnd + ":\n";

        return str;
    }

    std::string handleIf(Tree &tree){
        std::string ifLable = newLabel();
        std::string elseLable = newLabel();
        std::string var = newVar();
        std::string str = handleExpr(*tree.getChild()[2], var);
        str += "\tmov eax, [" + var + "]\n";
        str += "\tcmp eax, 0\n";
        str += "\tjz " + elseLable + "\n";
        str += handleStmt(*tree.getChild()[4]);
        str += "\tjmp " + ifLable + "\n";
        str += elseLable + ":\n";
        str += handleStmt(*tree.getChild()[6]);
        str += ifLable + ":\n";
        return str;
    }

    std::string handleExpr(Tree &tree, std::string &varAsm){
        return handleExprInner(*tree.getChild()[0], varAsm);
    }

    std::string handleExprInner(Tree &tree, std::string varAsm){
        switch (tree.getSymbol().noTerminalType){
            case NoTerminalType::EXPR_NUM:
                if(reg.find(varAsm) == reg.end())varAsm = std::string("[") + varAsm + "]";
                return std::string("\tmov dword ") + varAsm + ", " + \
                        std::to_string(tree.getChild()[0]->getTokenPtr()->getIntValue()) + "\n";
            case NoTerminalType::EXPR_ID:
                return movId(varTable[tree.getChild()[0]->getTokenPtr()->getName()], varAsm);
            case NoTerminalType::EXPR_ADD:{
                std::string first = newVar();
                std::string second = newVar();
                std::string str = handleExprInner(*tree.getChild()[0], first);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(second, "eax");
                str += "\tadd [" + first + "], eax\n";
                str += movId(first, varAsm);
                return str;
            };

            case NoTerminalType::EXPR_SUB:{
                std::string first = newVar();
                std::string second = newVar();
                std::string str = handleExprInner(*tree.getChild()[0], first);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(second, "eax");
                str += "\tsub [" + first + "], eax\n";
                str += movId(first, varAsm);
                return str;
            };
            case NoTerminalType::EXPR_MUL:{
                std::string first = newVar();
                std::string second = newVar();
                std::string str = handleExprInner(*tree.getChild()[0], first);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(second, "eax");
                str += "\tmul dword [" + first + "]\n";
                str += movId("eax", varAsm);
                return str;
            };
            case NoTerminalType::EXPR_DIV:{
                std::string first = newVar();
                std::string second = newVar();
                std::string str = handleExprInner(*tree.getChild()[0], first);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(first, "eax");
                str += "\tmov edx, 0\n";
                str += "\tdiv dword [" + second + "]\n";
                str += movId("eax", varAsm);
                return str;
            };
            case NoTerminalType::EXPR_MOD:{
                std::string first = newVar();
                std::string second = newVar();
                std::string str = handleExprInner(*tree.getChild()[0], first);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(first, "eax");
                str += "\tmov edx, 0\n";
                str += "\tdiv dword [" + second + "]\n";
                str += movId("edx", varAsm);
                return str;
            };


            case NoTerminalType::EXPR_LESS:{
                std::string second = newVar();
                std::string label = newLabel();
                std::string str = handleExprInner(*tree.getChild()[0], varAsm);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(second, "eax");
                str += "\tsub [" + varAsm + "], eax\n";
                str += "\tmov dword [" + varAsm + "], 0\n";
                str += "\tjns " + label + "\n";
                str += "\tmov dword [" + varAsm + "], 1\n";
                str += label + ":\n";
                return str;
            };
            case NoTerminalType::EXPR_GREATER:{
                std::string second = newVar();
                std::string label = newLabel();
                std::string str = handleExprInner(*tree.getChild()[0], varAsm);
                str += handleExprInner(*tree.getChild()[2], second);
                str += movId(second, "eax");
                str += "\tsub eax, [" + varAsm + "]\n";
                str += "\tmov dword [" + varAsm + "], 0\n";
                str += "\tjns " + label + "\n";
                str += "\tmov dword [" + varAsm + "], 1\n";
                str += label + ":\n";
                return str;
            };

            default:
                return "";
        }
    }
};