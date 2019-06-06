
#include <iostream>
#include <fstream>
#include <string>

#include "IR.h"
#include "CFG.h"
#include "BasicBlock.h"

IRInstr::IRInstr(BasicBlock *bb_, Operation op, TYPE t, vector<Variable> params)
{
    this->bb = bb_;
    this->op = op;
    this->t = t;
    this->params = params;
}

void IRInstr::gen_asm(ostream &ofs)
{
    updateAddresses();
    switch (this->op)
    {

    case parameters:
    {
        for (int i = 0; i < params.size(); i++)
        {
            ofs << "movl " << ARGS_REGISTERS[i] << ", " << this->params[i].value << "(%rbp)" << endl;
        }
        break;
    }

    case ldconst:
        //params contains the value to assign at [1] and the address where to write at [0]
        {
            if (this->params[0].type == INT)
            {
                if (this->params[1].number)
                { //ex: a = 2;
                    string content = "$" + to_string(this->params[1].value);
                    ofs << "movl " << content << ", " << this->params[0].value << "(%rbp)" << endl;
                }
                else
                { //ex: a = b;
                    string content = to_string(params[1].value) + "(%rbp)";
                    ofs << "movl " << content << ", %eax" << endl;
                    ofs << "movl %eax, " << this->params[0].value << "(%rbp)" << endl;
                }
            }
            else
            {
                if (this->params[1].number)
                { //ex: a = 2;
                    string content = "$" + to_string(this->params[1].value);
                    ofs << "movb " << content << ", " << this->params[0].value << "(%rbp)" << endl;
                }
                else
                { //ex: a = b;
                    string content = to_string(params[1].value) + "(%rbp)";
                    ofs << "movzbl " << content << ", %eax" << endl;
                    ofs << "movb %al, " << this->params[0].value << "(%rbp)" << endl;
                }
            }
            break;
        }

    case add:
    {
        Variable var0 = this->params[0];
        Variable var1 = this->params[1];
        string mov0;
        string mov1;
        if(var0.type == CHAR) mov0="movsbl ";
        else mov0="movl ";
        if(var1.type == CHAR) mov1="movsbl ";
        else mov1="movl ";
        if (var0.number && !var1.number)
        {
            ofs << mov1 << var1.value << "(%rbp) , %eax" << endl;
            ofs << "addl $" << var0.value << ", %eax" << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << mov0 << var0.value << "(%rbp) , %eax" << endl;
            ofs << "addl $" << var1.value << ", %eax" << endl;
        }
        else
        {
            ofs << mov0 << var0.value << "(%rbp) , %eax" << endl;
            ofs << mov1 << var1.value << "(%rbp) , %edx" << endl;
            ofs << "addl %edx, %eax" << endl;
        }
        //The case of adding two numbers is handled by visitor
        ofs << "movl %eax, " << params[2].value << "(%rbp)" << endl;
        break;
    }

    case sub:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string mov0;
        string mov1;
        if(var0.type == CHAR) mov0="movsbl ";
        else mov0="movl ";
        if(var1.type == CHAR) mov1="movsbl ";
        else mov1="movl ";
        if (var0.number && !var1.number) // 2 - a
        {
            if(var1.type == CHAR) {
                ofs << "movsbl " << var1.value << ", %eax" << endl;
                ofs << "movl $" << var0.value << ", %edx" << endl;
                ofs << "subl %eax, %edx" << endl;
                ofs << "movl %edx, %eax" << endl;
            }
            else {
                ofs << "movl $" << var0.value << ", %eax" << endl;
                ofs << "subl  " << var1.value << "(%rbp), %eax" << endl;
            }
        }
        else if (var1.number && !var0.number) // a - 2
        {
            ofs << mov0 << var0.value << "(%rbp) , %eax" << endl;
            ofs << "subl $" << var1.value << ", %eax" << endl;
        }
        else // a - b
        {
            ofs << mov0 << var0.value << "(%rbp) , %eax" << endl;
            ofs << mov1 << var1.value << "(%rbp) , %edx" << endl;
            ofs << "subl %edx, %eax" << endl;
        }
        //The case of subing two numbers is handled by visitor
        ofs << "movl %eax, " << params[2].value << "(%rbp)" << endl;
        break;
    }

    case mul:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string mov0;
        string mov1;
        if(var0.type == CHAR) mov0="movsbl ";
        else mov0="movl ";
        if(var1.type == CHAR) mov1="movsbl ";
        else mov1="movl ";
        if (var0.number && !var1.number)
        {
            ofs << mov1 << var1.value << "(%rbp) , %eax" << endl;
            ofs << "imull $" << var0.value << ", %eax, %eax" << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << mov0 << var0.value << "(%rbp) , %eax" << endl;
            ofs << "imull $" << var1.value << ", %eax, %eax" << endl;
        }
        else
        {
            if(var1.type == INT && var0.type == INT) {
                ofs << "movl " << var0.value << "(%rbp) , %eax" << endl;
                ofs << "imull " << var1.value << "(%rbp), %eax" << endl;
            }
            else {
                ofs << mov0 << var0.value << "(%rbp) , %edx" << endl;
                ofs << mov1 << var1.value << "(%rbp) , %eax" << endl;
                ofs << "imull %edx, %eax" << endl;
            }
        }
        //The case of multiplying two numbers is handled by visitor
        ofs << "movl %eax, " << params[2].value << "(%rbp)" << endl;
        break;
    }

    case div:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string mov0;
        string mov1;
        if(var0.type == CHAR) mov0="movsbl ";
        else mov0="movl ";
        if(var1.type == CHAR) mov1="movsbl ";
        else mov1="movl ";
        if (var0.number && !var1.number)
        {
            if(var1.type == INT){
                ofs << "movl $" << var0.value << ", %eax" << endl;
                ofs << "cdq" << endl; //or cldt if 64 bits
                ofs << "idivl " << var1.value << "(%rbp)" << endl;
            }
            else {
                ofs << "movsbl " << var1.value << ", %ecx" << endl;
                ofs << "movl $" << var0.value << ", %eax" << endl;
                ofs << "cdq" << endl; //or cldt if 64 bits
                ofs << "idivl ecx" << endl;
            }
        }
        else if (var1.number && !var0.number)
        {
            ofs << "movl $" << var1.value << ", %ecx" << endl;
            ofs << mov0 << var0.value << "(%rbp), %eax" << endl;
            ofs << "cdq" << endl;
            ofs << "idivl %ecx" << endl;
        }
        else
        {
            if(var1.type==INT) {
                ofs << mov0 << var0.value << "(%rbp), %eax" << endl;
                ofs << "cdq" << endl;
                ofs << "idivl " << var1.value << "(%rbp)" << endl;
            }
            else {
                ofs << mov0 << var0.value << "(%rbp), %eax" << endl;
                ofs << mov1 << var1.value << "(%rbp), %ecx" << endl;
                ofs << "cdq" << endl;
                ofs << "idivl %ecx" << endl;
            }
        }
        ofs << "movl %eax, " << params[2].value << "(%rbp)" << endl;
        //The case of dividing two numbers is handled by visitor
        break;
    }
    case ret:
    {
        Variable var = params[0];
        string content;
        TYPE returnType = this->bb->cfg->getReturnType();
        if (returnType == INT)
        {
            if (var.number)
                content = "$" + to_string(var.value);
            else
                content = to_string(var.value) + "(%rbp)";

            ofs << "movl " << content << ", %eax" << endl;
        }
        else
        {
            if (var.number)
                content = "$" + to_string(var.value);
            else
                content = to_string(var.value) + "(%rbp)";
            ofs << "movzbl " << content << ", %eax" << endl;
        }
        ofs << "movq %rbp, %rsp" << endl;
        ofs << "popq	%rbp " << endl;
        ofs << "ret" << endl;
        break;
    }

    case call:
    {

        string nameFunction = params[0].name;
        int start = 2;
        for (int i = start; i < params.size(); i++)
        {
            Variable var = params[i];
            string arg;
            if (var.number)
                arg = "$" + to_string(var.value);
            else
                arg = to_string(var.value) + "(%rbp)";

            ofs << "movl " << arg << ", " << ARGS_REGISTERS[i - start] << endl;
        }
        ofs << "call " << nameFunction << endl;
        ofs << "movl %eax, " << params[1].value << "(%rbp)" << endl;

        break;
    }

    case cmp_eq:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string cmp;
        string reg;
        if (var0.type == CHAR || var1.type == CHAR) cmp = "cmpb ";
        else cmp = "cmpl ";
        if (var0.type == CHAR && var1.type == CHAR) reg = "al";
        else reg = "eax";
        if (var0.number && !var1.number)
        {
            ofs << cmp << var0.asmString() << "," << var1.asmString() << endl;
            ofs << "jne ." << bb->exit_false->label << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << cmp << var1.asmString() << "," << var0.asmString() << endl;
            ofs << "jne ." << bb->exit_false->label << endl;
        }
        else
        {
            ofs << "movl " << var0.asmString() << ", %eax" << endl;
            ofs << cmp << var1.asmString() << ", %" << reg << endl;
            ofs << "jne ." << bb->exit_false->label << endl;
        }
        break;
    }
    case cmp_neq:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string cmp;
        string reg;
        if (var0.type == CHAR || var1.type ==CHAR ) cmp = "cmpb ";
        else cmp = "cmpl ";
        if (var0.type == CHAR && var1.type == CHAR) reg = "al";
        else reg = "eax";
        if (var0.number && !var1.number)
        {
            ofs << cmp << var0.asmString() << "," << var1.asmString() << endl;
            ofs << "je ." << bb->exit_false->label << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << cmp << var1.asmString() << "," << var0.asmString() << endl;
            ofs << "je ." << bb->exit_false->label << endl;
        }
        else
        {
            ofs << "movl " << var0.asmString() << ", %eax" << endl;
            ofs << cmp << var1.asmString() << ", %" << reg << endl;
            ofs << "je ." << bb->exit_false->label << endl;
        }
        break;
    }
    case cmp_le:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string cmp;
        string reg;
        if (var0.type == CHAR || var1.type ==CHAR ) cmp = "cmpb ";
        else cmp = "cmpl ";
        if (var0.type == CHAR && var1.type == CHAR) reg = "al";
        else reg = "eax";
        if (var0.number && !var1.number)
        {
            ofs << cmp << var0.asmString() << "," << var1.asmString() << endl;
            ofs << "js ." << bb->exit_false->label << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << cmp << var1.asmString() << "," << var0.asmString() << endl;
            ofs << "jg ." << bb->exit_false->label << endl;
        }
        else
        {
            ofs << "movl " << var0.asmString() << ", %eax" << endl;
            ofs << cmp << var1.asmString() << ", %" << reg << endl;
            ofs << "jg ." << bb->exit_false->label << endl;
        }
        break;
    }
    case cmp_lt:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
        string cmp;
        string reg;
        if (var0.type == CHAR || var1.type ==CHAR ) cmp = "cmpb ";
        else cmp = "cmpl ";
        if (var0.type == CHAR && var1.type == CHAR) reg = "al";
        else reg = "eax";
        if (var0.number && !var1.number)
        {
            ofs << cmp << var0.asmString() << "," << var1.asmString() << endl;
            ofs << "jle ." << bb->exit_false->label << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << cmp << "$" << (var1.value - 1) << ", " << var0.asmString() << endl;
            ofs << "jg ." << bb->exit_false->label << endl;
        }
        else
        {
            ofs << "movl " << var0.asmString() << ", %eax" << endl;
            ofs << cmp << var1.asmString() << ", %" << reg << endl;
            ofs << "jge ." << bb->exit_false->label << endl;
        }
        break;
    }

    case jmp_cond:
    {
        if (bb->exit_true != nullptr)
        {
            ofs << "jmp ." << bb->exit_true->label << endl;
        }
        else
        {
            ofs << "//Compiler asked for jmp_cond but there is no cond block" << endl;
        }
        break;
    }
    case jmp_self:
    {
        ofs << "jmp ." << bb->label << endl;
        break;
    }

    case jmp:
    {
        ofs << "jmp ." << params[0].name << endl;
        break;
    }
    case label:
    {
        ofs << "." << bb->label << ":" << endl;
        break;
    }
    case debug:
    {
        ofs << "//debug" << endl;
        break;
    }
    default:
        break;
    }
}

void IRInstr::updateAddresses()
{
    int i;
    for (i = 0; i < params.size(); i++)
    {
        if (!params[i].number && params[i].type == CHAR)
        {
            params[i].value += this->bb->cfg->getCharLocation();
        }
    }
}