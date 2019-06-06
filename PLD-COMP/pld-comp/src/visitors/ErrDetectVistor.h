#pragma once
//#define DEBUG

#include "antlr4-runtime.h"
#include "exprBaseVisitor.h"
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <string>

using namespace std;


class ErrDetectVisitor : public exprBaseVisitor{
public:

    //Creates a message error with the the msg and the line where it is located
    string createErrorMsg(string msg, antlr4::ParserRuleContext * ctx){
        stringstream ss;
        ss << "Error located at line " << ctx->getStart()->getLine() << ":" << endl;
        ss << "     "<< msg << endl;
        return ss.str();
    }

    void checkDeclarationVar(string nameVar, antlr4::ParserRuleContext * ctx){
        unordered_set<string>::const_iterator it = vars.find(nameVar);
        if(it != vars.end()){
            stringstream ss;
            ss<<"    Variable " << nameVar << " already declared" << endl;
            string error = createErrorMsg(ss.str(), ctx);
            throw error;
        }
    }


    antlrcpp::Any visitProg(exprParser::ProgContext *ctx) override{
        visitChildren(ctx);
        return nullptr;
    }

    antlrcpp::Any visitFunction_call(exprParser::Function_callContext*ctx) override { //func();
		#ifdef DEBUG
            cout<<"visiting function call of ";
            cout<<ctx->getText()<<endl;
        #endif

		//get name of the function to call
		string nameFunction = ctx->VAR()->getText();
        unordered_map<string,int>::const_iterator it = functions.find(nameFunction);
        //the function can exist even if not declared
        if(it==functions.end()){
            return nullptr;
        }
        int nbExpectedArguments = it->second;
        cout<<"expected "<<nbExpectedArguments<<endl;
		
        int nbArguments;
		if(!ctx->arguments()){
			nbArguments = 0;
        }
        else{
            nbArguments = ctx->arguments()->expr().size();
        }
        if(nbArguments != nbExpectedArguments){
            stringstream ss;
            ss<<"function "<<nameFunction<<" expects "<<nbExpectedArguments<<" not ";
            ss<<nbArguments <<" arguments";
            throw createErrorMsg(ss.str(),ctx);
        }
        return nullptr;
	}

    antlrcpp::Any visitFunction_declaration(exprParser::Function_declarationContext *ctx) override{
        #ifdef DEBUG
            cout<<"errdetect visiting function declaration "<<endl;
        #endif
		string nameFunction = ctx->VAR()->getText();
        int nbParameters;
        if(!ctx->parameters()){
            nbParameters = 0;
        }
        else{
            nbParameters = ctx->parameters()->parameter().size();
        }
        unordered_map<string,int>::const_iterator it = functions.find(nameFunction);
        if(it != functions.end()){
            stringstream ss;
            ss << "function "<<nameFunction<<" already declared";
            string error = createErrorMsg(ss.str(), ctx);
            throw error;
        }
        functions.insert(make_pair(nameFunction, nbParameters));
        vars.clear();
        visitChildren(ctx);
        return nullptr;
    }

    antlrcpp::Any visitParameters(exprParser::ParametersContext *ctx) override{
		#ifdef DEBUG
            cout<<"errdetect visiting parameters ";
			cout<<ctx->getText()<<endl;
        #endif
		for(auto it : ctx->parameter()){
			string nameVar = it->VAR()->getText();
			checkDeclarationVar(nameVar, ctx);
            vars.insert(nameVar);
		}
        cout<< ctx->parameter().size() <<endl;
        if(ctx->parameter().size() > 6){
            throw createErrorMsg("More than 6 parameters is not supported", ctx);
        }
		// // BasicBlock* currentBB=current->current_bb;
		// current->current_bb->add_IRInstr(IRInstr::parameters, defaultType, params); 
		return nullptr;
	}

    antlrcpp::Any visitStatement(exprParser::StatementContext *ctx) override {
        #ifdef DEBUG
            cout<<"ErrDetectVisitor visiting statement"<<endl;
        #endif
        visitChildren(ctx);
        return nullptr;
    }

    antlrcpp::Any visitDeclaration(exprParser::DeclarationContext *ctx) override { //int a=33; int a=x;
        #ifdef DEBUG
            cout<<"ErrDetectVisitor visiting declaration ";
            cout<<ctx->getText()<<endl;
        #endif
        string nameVar = ctx->VAR()->getText();
        checkDeclarationVar(nameVar, ctx);
        vars.insert(nameVar);

        return nullptr;
    }

    antlrcpp::Any visitDefinition(exprParser::DefinitionContext *ctx) override { //int a=33; int a=x;
        #ifdef DEBUG
            cout<<"ErrDetectVisitor visiting definition ";
            cout<<ctx->getText()<<endl;
        #endif
        string nameVar = ctx->VAR()->getText();
        checkDeclarationVar(nameVar, ctx);

        visit(ctx->expr());

        vars.insert(nameVar);

        return nullptr;
    }


    antlrcpp::Any visitAssignment(exprParser::AssignmentContext*ctx) override { //a=x
        #ifdef DEBUG
            cout<<"visiting Assignment ";
            cout<<ctx->getText()<<endl;
        #endif
        string nameVar = ctx->VAR()->getText();
        checkUseBeforeDeclaration(nameVar, ctx);
        visitChildren(ctx);
        return nullptr;
    }

    void checkUseBeforeDeclaration(string nameVar, antlr4::ParserRuleContext * ctx){
        unordered_set<string>::const_iterator it = vars.find(nameVar);
        if(it == vars.end()){
            stringstream ss;
            ss<<"    Variable " << nameVar << " used before being declared" << endl;
            string error = createErrorMsg(ss.str(), ctx);
            throw error;
        }
    }

    antlrcpp::Any visitVar(exprParser::VarContext *ctx) override {
		#ifdef DEBUG
            cout<<"ErrDetectVisitor visting Var ";
            cout<<ctx->getText()<<endl;
        #endif
        string nameVar = ctx->VAR()->getText();
        checkUseBeforeDeclaration(nameVar, ctx);

		return nullptr;
	}


private:
    unordered_set<string> vars;
    unordered_map<string, int> functions; //int indicates the nb of arguments



};