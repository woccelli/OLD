#ifndef CFG_H
#define CFG_H

using namespace std;

#include <vector>
#include <map>
#include <string>
#include <stack>
#include "BasicBlock.h"

class Variable;

const int SIZE_INT = 4;
const int SIZE_CHAR = 1;

class CFG {
 public:
	CFG(string label, TYPE t){
		name = label;
		returnType = t;
        BasicBlock * bb = new BasicBlock(this, label);
        current_bb = bb;

		bb_list.push_back(bb);

		indexInt = 0;
		intOffset = 0;
		intLocation = 0;
		maxIntOffset = 0;
		maxCharOffset = 0;
		indexChar = 0;
		charOffset = 0;
		charLocation = 0;
		currentLabelNumber = 0;
    }

	void gen_asm_prologue(ofstream& ofs){
		//updateCharLocation();
		this->charLocation = indexInt+maxIntOffset;
        ofs << ".text" << endl;
        ofs << ".global " << name << endl;
        ofs << name << ":" << endl;
        ofs << "pushq	%rbp" << endl;
        ofs << "movq	%rsp, %rbp" << endl;
		int offsetRsp = -indexInt - indexChar - maxIntOffset - maxCharOffset;
		offsetRsp += (16-offsetRsp % 16);
		ofs << "subq $"<< offsetRsp << "," << "%rsp" <<endl;
    }

	void gen_asm(ofstream& ofs){
		for (auto it = bb_list.begin(); it != bb_list.end(); ++it){
    		(*it)->gen_asm(ofs);
		}
    }

	void gen_asm_epilogue(ofstream& ofs){
        ofs << "// end of CFG" << endl;
    }

	void gen_asm_prologue_msp430(ofstream& ofs){
		
        ofs << "PUSHM.W #1, R4" << endl;
        ofs << "MOV.W R1, R4 " <<  endl;
        ofs << "SUB.W #" << 2*Symbol.size() << ", R1" <<  endl;
    }

	void gen_asm_msp430(ofstream& ofs){
		for (auto it = bb_list.begin(); it != bb_list.end(); ++it){
    		(*it)->gen_asm_msp430(ofs);
		}
    }

	void gen_asm_epilogue_msp430(ofstream& ofs){
		ofs << "ADD.W #" << 2*Symbol.size() << ", R1" << endl;
        ofs << "POPM.W #1, R4" << endl;
        ofs << "RET" <<  endl;
        
    }

	// symbol table methods
	int create_variable(string name, TYPE t){
		if(t == 0){
			indexInt -= SIZE_INT;
			Symbol.insert(make_pair(name, indexInt));
			SymbolType.insert(make_pair(name, INT));
			return indexInt;
		}else{
			indexChar -= SIZE_CHAR;
			Symbol.insert(make_pair(name, indexChar));
			SymbolType.insert(make_pair(name, CHAR));
			return indexChar;
		}
    }

	int create_temp_variable(TYPE t){
		if(t == 0){
			intOffset -= SIZE_INT;
			if(maxIntOffset>intOffset) maxIntOffset = intOffset;
			return indexInt+intOffset;
		}else{
			charOffset -= SIZE_CHAR;
			if(maxCharOffset>charOffset) maxCharOffset = charOffset;
			return indexChar+charOffset;
		}
	}

	int get_variable_address(string name){
		return Symbol.find(name)->second;
	}

	TYPE get_variable_type(string name){
		return SymbolType.find(name)->second;
	}

	string findVariable(int address){
		map <string, int>::iterator it;
		for(it = Symbol.begin(); it!=Symbol.end();++it){
			if(it->second==address) return it->first;
		}
		return "";
	}

	// void updateCharLocation() {
	// 	map <string, TYPE>::iterator it;
	// 	for(it = SymbolType.begin(); it!=SymbolType.end();++it){
	// 		if(it->second==CHAR) Symbol.find(it->first)->second = Symbol.find(it->first)->second+charLocation;
	// 	}
	// }

	int getCharLocation(){
		return charLocation;
	}

	void reset_offsets(){
		intOffset = 0;
		charOffset = 0;
	}

	void divide_current(){
		BasicBlock * cond = new BasicBlock(this, genLabelName());
		BasicBlock * uncond = new BasicBlock(this, genLabelName());
		BasicBlock * finalBlock = new BasicBlock(this, genLabelName());


		if(current_bb->exit_true != nullptr){
			finalBlock->exit_true = current_bb->exit_true;
		}

		current_bb->exit_true = cond;
		current_bb->exit_false = uncond;
		cond->exit_true = finalBlock;
		cond->exit_false = finalBlock;
		uncond->exit_true = finalBlock;

		bb_list.push_back(cond);
		bb_list.push_back(uncond);
		bb_list.push_back(finalBlock);


		current_bb = cond;
		alt_bb.push(uncond);
	}

	bool switch_branch(){
		if(alt_bb.empty()) return false;
		current_bb = alt_bb.top();
		alt_bb.pop();
		return true;
	}

	void merge_branch(){
		current_bb = current_bb->exit_true;
	}

	stack<BasicBlock* > alt_bb;
	BasicBlock* current_bb;

	string genLabelName(){
		return "L" + to_string(currentLabelNumber++);
	}

	TYPE getReturnType(){
		return returnType;
	}

	void showGraph(){
		for (auto it = bb_list.begin(); it != bb_list.end(); ++it){
			cout << (*it)->label << ",cond : ";
    		if((*it)->exit_true == nullptr){
				cout << "NULL";
			}else{
				cout << (*it)->exit_true->label;
			}
			cout << ", uncond : ";
			if((*it)->exit_false == nullptr){
				cout << "NULL";
			}else{
				cout << (*it)->exit_false->label;
			}
			cout << endl;
		}
	}

protected:
	string name; //name of the function
	TYPE returnType;
	map <string, int> Symbol; /**< part of the symbol table  */
	map <string, TYPE> SymbolType; /**< part of the symbol table  */
	//int nextFreeSymbolIndex; /**< to allocate new symbols in the symbol table */
	//int nextBBnumber; /**< just for naming */
	int indexInt;
    int intOffset;
	int intLocation;
	int maxIntOffset;
	int maxCharOffset;
	int indexChar;
	int charOffset;
	int charLocation;

	int currentLabelNumber; 

	BasicBlock* root_bb; /**< all the basic blocks of this CFG*/
	vector<BasicBlock *> bb_list;
};

#endif