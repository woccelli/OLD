#include <iostream>
#include <fstream>
#include <string>

#include "IR.h"
#include "BasicBlock.h"

const int coefAddress = 16;


void IRInstr::gen_asm_msp430(ostream &ofs)
{
    switch (this->op)
    {

    case parameters:
    {
        //non traité
        break;
    }
    case ldconst:
        //params contains the value to assign at [1] and the address where to write at [0]
        {
            if (this->params[1].number)
            { //ex: a = 2;p
                ofs << "MOV.W #" << this->params[1].value << ", " << this->params[0].value/coefAddress << "(R4)" << endl;
            }
            else
            { //ex: a = b;
                ofs << "MOV.W " << this->params[1].value/coefAddress << "(R4), " << this->params[0].value/coefAddress << "(R4)" << endl;
            }
            break;
        }

    case add:
    {
        Variable var0 = this->params[0];
        Variable var1 = this->params[1];
        if (var0.number && !var1.number)
        {
            ofs << "MOV.W " << var1.value/coefAddress << "(R4), R12" << endl;
            ofs << "ADD.W #" << var0.value << ", R12" << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << "MOV.W " << var0.value/coefAddress << "(R4), R12" << endl;
            ofs << "ADD.W #" << var1.value << ", R12" << endl;
        }
        else
        {
            ofs << "MOV.W " << var0.value/coefAddress << "(R4), R12" << endl;
            ofs << "ADD.W " << var1.value/coefAddress << "(R4), R12" << endl;
        }
        //The case of adding two numbers is handled by visitor
        ofs << "MOV.W R12, " << this->params[2].value/coefAddress << "(R4)" << endl;
        break;
    }

    case sub:
    {
        Variable var0 = params[0];
        Variable var1 = params[1];
       if (var0.number && !var1.number)
        {
            ofs << "MOV.W " << var1.value/coefAddress << "(R4), R12" << endl;
            ofs << "SUB.W #" << var0.value << ", R12" << endl;
        }
        else if (var1.number && !var0.number)
        {
            ofs << "MOV.W " << var0.value/coefAddress << "(R4), R12" << endl;
            ofs << "SUB.W #" << var1.value << ", R12" << endl;
        }
        else
        {
            ofs << "MOV.W " << var0.value/coefAddress << "(R4), R12" << endl;
            ofs << "SUB.W " << var1.value/coefAddress << "(R4), R12" << endl;
        }

        ofs << "MOV.W R12, " << this->params[2].value/coefAddress << "(R4)" << endl;
        break;
    }

    case mul:
    {
        //non traité
        break;
    }

    case div:
    {
        //non traité
        break;
    }
    case ret:
    {
        Variable var = params[0];
        string content;
        if (var.number)
            ofs << "MOV.W #" << var.value << ", R12" << endl;
        else
            ofs << "MOV.W " << var.value/coefAddress << "(R4), R12" << endl;
        break;
    }

    case call:
    {
        
        //non traité


        break;
    }
    case cmp_eq:
    {
        //non traité
        break;
    }    
    case cmp_neq:
    {
        //non traité
        break;
    }    
    case cmp_le:    
    {
        //non traité
        break;
    }
    case cmp_lt:
    {
        //non traité
        break;
    }

    default:
        break;
    }
}