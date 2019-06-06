#ifndef IR_H
#define IR_H

using namespace std;

#include <vector>
#include <map>
#include <string>
class CFG;

typedef enum{
		INT,
		CHAR
} TYPE;

class Variable{
	public:
	Variable(){}

	Variable(bool num, TYPE t, int val, string fname=""){
		number = num;
		type = t;
		value = val;
		name = fname;
	}

	string asmString(){
		if(number){
			return "$" + to_string(value);
		}else{
			return to_string(value) + "(%rbp)"; 
		}
	}

	bool number; //number if true, adress if false
	int value;
	TYPE type;
	//name used only for function calls
	string name;
};

// Declarations from the parser -- replace with your own

class BasicBlock;
class CFG;

//! The class for one 3-address instruction
class IRInstr {
 
   public:
	/** The instructions themselves -- feel free to subclass instead */
	typedef enum {
		parameters,
		ldconst,
		add,
		sub,
		mul,
		div,
		rmem,
		wmem,
		call, 
		cmp_eq,
		cmp_neq,
		cmp_lt,
		cmp_le,
		jmp_cond,
		jmp_self,
		jmp,
		label,
		ret,
		debug
	} Operation;


	/**  constructor */
	IRInstr(BasicBlock* bb_, Operation op, TYPE t, vector<Variable> params);
	
	/** Actual code generation */
	void gen_asm(ostream &o); /**< x86 assembly code generation for this IR instruction */
	void gen_asm_msp430(ostream &o); /**< msp430 assembly code generation for this IR instruction */

	void updateAddresses(); /** updare addresses of all char in parameters; */
	vector<Variable> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */
	
 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Operation op;
	TYPE t;
	int result;
	const string ARGS_REGISTERS[6] = {"%edi","%esi", "%edx", "%ecx", "%r8d", "%r9d"};
	// if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very important) comment becomes useless: it would be a better design. 
};

#endif
