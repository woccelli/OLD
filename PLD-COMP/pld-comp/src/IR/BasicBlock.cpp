#include "BasicBlock.h"

BasicBlock::BasicBlock(CFG* cfg, string entry_label){
    this->cfg=cfg;
    this->label=entry_label;
    this->instrs = vector<IRInstr*>(0);
    this->exit_false = nullptr;
    this->exit_true = nullptr;
}

void BasicBlock::gen_asm(ostream &ofs)
{
    /*if(this->label != "main"){
        ofs<<"."<< this->label << ":" << endl;
    }*/
    for (auto const &instr : this->instrs)
    {
        instr->gen_asm(ofs);
    }
    /**if(exit_false == nullptr && exit_true != nullptr){
        ofs<< "jmp ."<< exit_true->label<< endl; 
    }*/
}
void BasicBlock::gen_asm_msp430(ostream &ofs)
{
    for (auto const &instr : this->instrs)
    {
        instr->gen_asm_msp430(ofs);
    }
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, TYPE t, vector<Variable> params)
{
    IRInstr *newInstr = new IRInstr(this, op, t, params);
    this->instrs.push_back(newInstr);
}