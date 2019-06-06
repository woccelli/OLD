#pragma once

#include "antlr4-runtime.h"
#include "exprBaseVisitor.h"
#include "../IR/IR.h"
#include "../IR/CFG.h"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
//#define DEBUG
using namespace std;
TYPE defaultType = INT;


class Visitor : public exprBaseVisitor{
public:
	antlrcpp::Any visitProg(exprParser::ProgContext *ctx) override{
        #ifdef DEBUG
            cout<<"visiting prog"<<endl;
        #endif
		visitChildren(ctx);
		//current->showGraph();
		return Cfgs;
	}

	antlrcpp::Any visitFunction_declaration(exprParser::Function_declarationContext *ctx) override{
        #ifdef DEBUG
            cout<<"visiting function declaration "<<endl;
        #endif
		string nameFunction = ctx->VAR()->getText();
		string typeFunction = ctx->type()->getText();
		if(typeFunction=="int") current = new CFG(nameFunction, INT);
		else current = new CFG(nameFunction, CHAR);
		Cfgs.push_back(current);
        visitChildren(ctx);
        return nullptr;
    }

	antlrcpp::Any visitParameters(exprParser::ParametersContext *ctx) override{
		#ifdef DEBUG
            cout<<"visiting parameters ";
			cout<<ctx->getText()<<endl;
        #endif
		vector<Variable> params;
		for(auto it : ctx->parameter()){
			string nameVar = it->VAR()->getText();
			int varAddress = current->create_variable(nameVar, INT);
			params.push_back(Variable(false, defaultType, varAddress));
		}
		// BasicBlock* currentBB=current->current_bb;
		current->current_bb->add_IRInstr(IRInstr::parameters, defaultType, params); 
		return nullptr;
	}

    antlrcpp::Any visitStatement(exprParser::StatementContext *ctx) override {
        #ifdef DEBUG
            cout<<"visiting statement"<<endl;
        #endif

        visitChildren(ctx);
        return nullptr;
    }

	antlrcpp::Any visitIf_stat(exprParser::If_statContext *ctx) override {
		#ifdef DEBUG
            cout<<"visiting If_stat"<<endl;
        #endif
		
		Variable var = visit(ctx->expr()).as<Variable>();
		vector<Variable> params(0);
		
		if(var.number){
			if(var.value!=0) visit(ctx->block(0));
			else if(ctx->block(1) != nullptr) visit(ctx->block(1));
		}else{
			BasicBlock* currentBB=current->current_bb;
			current->divide_current();
			currentBB->add_IRInstr(IRInstr::jmp_cond,defaultType,params);
			
			current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
			visit(ctx->block(0));
			current->current_bb->add_IRInstr(IRInstr::jmp_cond,defaultType,params);
			
			current->switch_branch();
			current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
			if(ctx->block(1) != nullptr) visit(ctx->block(1));
			current->current_bb->add_IRInstr(IRInstr::jmp_cond,defaultType,params);
			
			current->merge_branch();
			current->current_bb->add_IRInstr(IRInstr::debug,defaultType,params);
		current->current_bb->add_IRInstr(IRInstr::debug,defaultType,params);
		
			current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
		}

    	return nullptr;
  	}

	antlrcpp::Any visitWhile_stat(exprParser::While_statContext *ctx) override {
		#ifdef DEBUG
            cout<<"visitWhile_stat"<<endl;
        #endif

		vector<Variable> params(0);
		
		BasicBlock* currentBB=current->current_bb;
		current->divide_current();
		currentBB->add_IRInstr(IRInstr::jmp_cond,defaultType,params);
		current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
		string loop_label = current->current_bb->label;
		string exit_label = current->current_bb->exit_true->label;
		Variable var = visit(ctx->expr()).as<Variable>();

		currentBB=current->current_bb;
		current->divide_current();
		currentBB->add_IRInstr(IRInstr::jmp_cond,defaultType,params);
		current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
		if(var.number){
			if(var.value!=0) visit(ctx->block());
		}else{
			visit(ctx->block());
			vector<Variable> params2{Variable(true, INT, 0,loop_label)};
			current->current_bb->add_IRInstr(IRInstr::jmp,defaultType,params2);
		}
		current->switch_branch();
		current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);
		vector<Variable> params2{Variable(true, INT, 0,exit_label)};
		current->current_bb->add_IRInstr(IRInstr::jmp,defaultType,params2);
		current->merge_branch();

		current->switch_branch();
		current->merge_branch();
		current->current_bb->add_IRInstr(IRInstr::debug,defaultType,params);

		current->current_bb->add_IRInstr(IRInstr::label,defaultType,params);

    	return nullptr;
	}

	antlrcpp::Any visitDeclaration(exprParser::DeclarationContext *ctx) override { //int a;
		#ifdef DEBUG
            cout<<"visiting declaration"<<endl;
            cout<<ctx->getText()<<endl;
        #endif

        string nameVar = ctx->VAR()->getText();
		string typeVar = ctx->type()->getText();
		if(typeVar == "int") current->create_variable(nameVar, INT);
		else if(typeVar == "char") current->create_variable(nameVar, CHAR);
		current->create_variable(nameVar, INT);
		return nullptr;
  	}

    antlrcpp::Any visitDefinition(exprParser::DefinitionContext *ctx) override { //int a=33; int a=x;
        #ifdef DEBUG
            cout<<"visiting definition"<<endl;
            cout<<ctx->getText()<<endl;
        #endif

        string nameVar = ctx->VAR()->getText();
		string typeVar = ctx->type()->getText();
		int varAddress;

		if(typeVar == "int") varAddress = current->create_variable(nameVar, INT);
		else if(typeVar == "char") varAddress = current->create_variable(nameVar, CHAR);
        //Get the children
		Variable var = visit(ctx->expr()).as<Variable>();
		vector<Variable> params;
		if(typeVar == "int") params = {Variable(false, INT, varAddress), var};
		else if(typeVar == "char") params = {Variable(false, CHAR, varAddress), var};
		BasicBlock* currentBB=current->current_bb;
		if(typeVar == "int") currentBB->add_IRInstr(IRInstr::ldconst, INT, params); //params[0] <- params[1]
		else if(typeVar == "char") currentBB->add_IRInstr(IRInstr::ldconst, CHAR, params);
		current->reset_offsets();
        return nullptr;
    }

    antlrcpp::Any visitAssignment(exprParser::AssignmentContext*ctx) override { //a=x
        #ifdef DEBUG
            cout<<"visiting Assignment"<<endl;
            cout<<ctx->getText()<<endl;
        #endif
        
        string nameVar = ctx->VAR()->getText();
		
        int indexVar = current->get_variable_address(nameVar);
		TYPE typeVar = current->get_variable_type(nameVar);
		auto op0 = ctx->expr();
		Variable var = visit(op0).as<Variable>();
		vector<Variable> params{Variable(false, typeVar, indexVar), var};
		BasicBlock* currentBB = current->current_bb;
		currentBB->add_IRInstr(IRInstr::ldconst, typeVar, params); //params[0] <- params[1]
		current->reset_offsets();
        return nullptr;
    }

	antlrcpp::Any visitCall(exprParser::CallContext*ctx) override {
		#ifdef DEBUG
            cout<<"visiting call ";
            cout<<ctx->getText()<<endl;
        #endif
		return visit(ctx->function_call());
	}

	antlrcpp::Any visitFunction_call(exprParser::Function_callContext*ctx) override { //func();
		#ifdef DEBUG
            cout<<"visiting function call of ";
            cout<<ctx->getText()<<endl;
        #endif
		//get name of the function to call
		string nameFunction = ctx->VAR()->getText();
		//create a tmp address where the return value will be stored
		int tempAddress  = current->create_temp_variable(INT);
		//extract the arguments if there are any
		vector<exprParser::ExprContext*> args;
		if(ctx->arguments())
			args = ctx->arguments()->expr();
		//create a vector which will be passed to the IR
		vector<Variable> params(args.size() + 2);
		//first param is the name of the function to call
		params[0] = Variable(0, defaultType,0,nameFunction);
		//second is the adress of the return value
		params[1] = Variable(true, defaultType,tempAddress);
		//for each argument calculate the expresion and insert it in params
		for(int it = 2; it < params.size(); it++){
			auto op = args[it-2];
			Variable argFromExpr = visit(op).as<Variable>();
			params[it] = argFromExpr;
		}
		BasicBlock* currentBB = current->current_bb;
		currentBB->add_IRInstr(IRInstr::call, defaultType, params);

		//returning the value
		Variable a(false, defaultType, tempAddress);
    	antlrcpp::Any any(a);
		return any; 
	}

	antlrcpp::Any visitInt(exprParser::IntContext *ctx) override {
		#ifdef DEBUG
            cout<<"vist Int"<<endl;
        #endif

		Variable a(true, INT, stoi(ctx->INT()->getText()));
		antlrcpp::Any any(a);
		return any;
	}
	
	antlrcpp::Any visitChar(exprParser::CharContext *ctx) override { // '*'
		#ifdef DEBUG
            cout<<"vist Char"<<endl;
        #endif
		
		//Get the value of the char between quotes and convert it to its ASCII value
		Variable a(true, CHAR, (int)ctx->CHAR()->getText()[1]);
		antlrcpp::Any any(a);
		return any;
	}
	
	antlrcpp::Any visitSpecialChar(exprParser::SpecialCharContext *ctx) override { // '*'
		#ifdef DEBUG
            cout<<"vist SpecialChar"<<endl;
        #endif
		
		//Get the value of the char between quotes and convert it to its ASCII value
		string specialChar = ctx->SPECIAL_CHAR()->getText();
		Variable a(true, CHAR, 0);
		cout << a.value << endl;
		if (specialChar == "'\\n'") a.value = 10;
		else if (specialChar == "'\\r'") a.value = 13;
		else if (specialChar == "'\\t'") a.value = 9;
		else if (specialChar == "'\\v'") a.value = 11;
		else if (specialChar == "'\\b'") a.value = 8;
		else if (specialChar == "'\\f'") a.value = 12;
		else if (specialChar == "'\\a'") a.value = 7;
		cout << a.value << endl;
		antlrcpp::Any any(a);
		return any;
	}
	
	antlrcpp::Any visitVar(exprParser::VarContext *ctx) override {
		#ifdef DEBUG
            cout<<"vist Var"<<endl;
        #endif

		TYPE varType = current->get_variable_type(ctx->getText());
		Variable a(false, varType, current->get_variable_address(ctx->getText()));
		antlrcpp::Any any(a);
		return any;
	}

    antlrcpp::Any visitReturn_statement(exprParser::Return_statementContext *ctx) override {
        #ifdef DEBUG
            cout<<"Return_Statement"<<endl;
        #endif

		auto op0 = ctx->expr();
		Variable var = visit(op0).as<Variable>();
		vector<Variable> params{var};
		BasicBlock* currentBB = current->current_bb;
		//currentBB->add_IRInstr(IRInstr::ret, defaultType, params); // params[0] = return value
		if(var.number) {
			currentBB->add_IRInstr(IRInstr::ret, defaultType, params);
		}
		else {
			TYPE varType = current->get_variable_type(current->findVariable(var.value));
			currentBB->add_IRInstr(IRInstr::ret, varType, params);
		}
        return nullptr;
    }
  	
	antlrcpp::Any visitAddSub(exprParser::AddSubContext *ctx) override {
		#ifdef DEBUG
            cout<<"visitAddSub"<<endl;
        #endif

		string op = ctx->op->getText();
		auto op0 = ctx->expr(0);
    	auto op1 = ctx->expr(1);
    	Variable var0 = visit(op0).as<Variable>();
    	Variable var1 = visit(op1).as<Variable>();

		if(var0.number && var1.number) {
			int temp = 0;
			if(op == "+") temp = var0.value + var1.value;
			else temp = var0.value - var1.value;

			Variable a(true, defaultType, temp);
			antlrcpp::Any any(a);
			return any;
		}

		if(!var0.number) var0.type = current->get_variable_type(current->findVariable(var0.value));
		if(!var1.number) var1.type = current->get_variable_type(current->findVariable(var1.value));
		
		int tempAddress  = current->create_temp_variable(INT);
		vector<Variable> params{var0, var1, Variable(true, defaultType, tempAddress)};
		BasicBlock* currentBB=current->current_bb;
		if(op == "+"){
			currentBB->add_IRInstr(IRInstr::add, defaultType, params);
		}else{
			currentBB->add_IRInstr(IRInstr::sub, defaultType, params);
		}
	
		Variable a(false, defaultType, tempAddress);
    	antlrcpp::Any any(a);
		return any;
 	}

	antlrcpp::Any visitMultDiv(exprParser::MultDivContext *ctx) override {
		#ifdef DEBUG
            cout<<"visitMultDiv"<<endl;
        #endif

		string op = ctx->op->getText();
    	Variable var0 = visit(ctx->expr(0)).as<Variable>();
    	Variable var1 = visit(ctx->expr(1)).as<Variable>();
		cout << var0.type << var1.type << endl;

		if(var0.number && var1.number) {
			int temp = 0;
			if(op == "/") temp = var0.value / var1.value;
			else temp = var0.value * var1.value;

			Variable a(true, defaultType, temp);
			antlrcpp::Any any(a);
		
			return any;
		}

		if(!var0.number && var0.type == INT) var0.type = current->get_variable_type(current->findVariable(var0.value));
		if(!var1.number && var1.type == INT) var1.type = current->get_variable_type(current->findVariable(var1.value));
		cout << var0.type << var1.type << endl;
		

		int tempAddress  = current->create_temp_variable(INT);
		vector<Variable> params{var0, var1, Variable(true, defaultType, tempAddress)};
		BasicBlock* currentBB=current->current_bb;
		if(op == "/"){
			currentBB->add_IRInstr(IRInstr::div, defaultType, params);
		}else{
			currentBB->add_IRInstr(IRInstr::mul, defaultType, params);
		}

		Variable a(false, defaultType, tempAddress);
    	antlrcpp::Any any(a);
		return any; 
 	}

	antlrcpp::Any visitPar(exprParser::ParContext *ctx) override {
    	#ifdef DEBUG
            cout<<"visit Par"<<endl;
        #endif

        auto op0 = ctx->expr();
		antlrcpp::Any any = visit(op0);
		return any;
  	}

	antlrcpp::Any visitComparison(exprParser::ComparisonContext *ctx) override {
		#ifdef DEBUG
            cout<<"visitComparison"<<endl;
        #endif

		string op = ctx->op->getText();
    	Variable var0 = visit(ctx->expr(0)).as<Variable>();
    	Variable var1 = visit(ctx->expr(1)).as<Variable>();

		if(var0.number && var1.number) {
			int temp = 0;
			int v1 = var0.value;
			int v2 = var1.value;

			if(op == "==" && v1==v2) temp = 1;
			else if(op == "!=" && v1!=v2) temp = 1;
			else if(op == "<" && (v1 < v2)) temp = 1;
			else if(op == ">" && (v1 > v2)) temp = 1;
			else if(op == "<=" && (v1 <= v2)) temp = 1;
			else if(op == ">=" && (v1 >= v2)) temp = 1;

			Variable a(true, defaultType, temp);
			antlrcpp::Any any(a);
		
			return any;
		}

		if(!var0.number) var0.type = current->get_variable_type(current->findVariable(var0.value));
		if(!var0.number) var1.type = current->get_variable_type(current->findVariable(var1.value));
		
		int tempAddress  = current->create_temp_variable(defaultType);
		vector<Variable> params{var0, var1, Variable(true, defaultType, tempAddress)};
		vector<Variable> paramsInv{var1, var0, Variable(true, defaultType, tempAddress)};
		BasicBlock* currentBB=current->current_bb;


		if(op == "=="){
			currentBB->add_IRInstr(IRInstr::cmp_eq, defaultType, params);
		}else if(op == "!="){
			currentBB->add_IRInstr(IRInstr::cmp_neq, defaultType, params);
		}else if(op == "<"){
			currentBB->add_IRInstr(IRInstr::cmp_lt, defaultType, params);
		}else if(op == ">"){
			currentBB->add_IRInstr(IRInstr::cmp_lt, defaultType, paramsInv);
		}else if(op == "<="){
			currentBB->add_IRInstr(IRInstr::cmp_le, defaultType, params);
		}else if(op == ">="){
			currentBB->add_IRInstr(IRInstr::cmp_le, defaultType, paramsInv);
		}

		Variable a(false, defaultType, tempAddress);
		antlrcpp::Any any(a);
		return any;
	}

	virtual antlrcpp::Any visitLogicalOperators(exprParser::LogicalOperatorsContext *ctx) override {
    	#ifdef DEBUG
            cout<<"visitLogicalOperators"<<endl;
        #endif

		string op = ctx->op->getText();
    	Variable var0 = visit(ctx->expr(0)).as<Variable>();
    	Variable var1 = visit(ctx->expr(1)).as<Variable>();

		//(ex. 2 && 3, 4 || 5...)
		if(var0.number && var1.number) {
			int temp = 0;
			int v1 = var0.value;
			int v2 = var1.value;

			if(op == "&&" && v1!=0 && v2!=0) temp = 1;
			else if(op == "||" && (v1!=0 || v2!=0)) temp = 1;

			Variable a(true, defaultType, temp);
			antlrcpp::Any any(a);
		
			return any;
		}
		
		int tempAddress  = current->create_temp_variable(INT);
		vector<Variable> params{var0, var1, Variable(true, defaultType, tempAddress)};
		vector<Variable> paramsInv{var1, var0, Variable(true, defaultType, tempAddress)};
		BasicBlock* currentBB=current->current_bb;
		//currentBB->add_IRInstr(IRInstr::label, defaultType, params);
		if(op == "&&"){
			currentBB->add_IRInstr(IRInstr::cmp_eq, defaultType, params);
		}else if(op == "||"){
			currentBB->add_IRInstr(IRInstr::cmp_neq, defaultType, params);
		}

		Variable a(false, defaultType, tempAddress);
		antlrcpp::Any any(a);
		return any;
  	}



private:	
	vector<CFG*> Cfgs;
	CFG * current;	
};
