#include <iostream>
#include <fstream>
#include <string.h>
#include <streambuf>

#include "antlr4-runtime.h"
#include "exprLexer.h"
#include "exprParser.h"

#include "dotexport.h"
#include "visitors/CodeGenVisitor.h"
#include "visitors/ErrDetectVistor.h"

using namespace antlr4;

int main(int argc, char *argv[]) {
  std::ifstream ifs(argv[1], std::ifstream::in);
  std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

  bool staticAnalysis = false;
  bool optimise = false;
  bool generateAssembly = false;
  bool generateAssemblyMsp = false;
  char* outputFile = (char*)"test.s";
  for (int i = 0; i < argc; ++i) {
      if (!strcmp((char*)"-a",argv[i])) {
          staticAnalysis = true;
          std::cout << "Static analysis option activated" << std::endl;
      }
      if (!strcmp((char*)"-o", argv[i])) {
          optimise = true;
          std::cout << "Optimisation option activated" << std::endl;
      }
      if (!strcmp((char*)"-s", argv[i])) {
          generateAssembly = true;
          std::cout << "Generate assembly option activated" << std::endl;
          if (i < argc - 1) {
              outputFile = argv[i + 1];
          }
      }
      if (!strcmp((char*)"-msp", argv[i])) {
          generateAssemblyMsp = true;
          std::cout << "Generate assembly for msp430 option activated" << std::endl;
          if (i < argc - 1) {
              outputFile = argv[i + 1];
          }
      }
  }

  ANTLRInputStream input(str);
  exprLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  tokens.fill();
//   for (auto token : tokens.getTokens()) {
//     cout << token->toString() << std::endl;
//   }

  exprParser parser(&tokens);
  tree::ParseTree *tree = parser.prog();
  //std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
  if(lexer.getNumberOfSyntaxErrors() > 0){
      cerr<<"compilation aborted because of lexical errors"<<endl;
      return EXIT_FAILURE;
  }
  
  if(parser.getNumberOfSyntaxErrors() > 0){
      cerr<<"compilation aborted because of syntax errors"<<endl;
      return EXIT_FAILURE;
  }
  //visitor for detecting errors in the program
  if(staticAnalysis){
    cout<<"starting static analysis"<<endl;
    ErrDetectVisitor errDetectVisitor;
    try{
        errDetectVisitor.visit(tree);
    }
    catch(string msg){
        cerr << msg << endl;
        return EXIT_FAILURE;
    }
    cout<<"static analysis done"<<endl;
  }
  if(generateAssembly){
    //Main visitor
    Visitor VisitorI;
    vector<CFG*> Cfgs = VisitorI.visit(tree);
    ofstream ofs;
    ofs.open(outputFile, ofstream::trunc);
    for(CFG* cfg : Cfgs){
        cfg->gen_asm_prologue(ofs);
        cfg->gen_asm(ofs);
        cfg->gen_asm_epilogue(ofs);
    }

        DotExport dotexport(&parser);
        tree::ParseTreeWalker::DEFAULT.
                walk(&dotexport, tree
        );
        ofstream out;
  }
/*
out.open("./graph/tmp.dot");
out<<dotexport.getDotFile();
out.close();
system("dot -Tpdf -o ./graph/out.pdf ./graph/tmp.dot");
*/

    return 0;
}