ANTLR=../ANTLR4-CPP/bin/antlr4
ANTLRRUNTIME=../ANTLR4-CPP/
RESULTDIR=results


default: mkantlr mkmain mkexec

mkantlr: 
	$(ANTLR) -visitor -no-listener -Dlanguage=Cpp  -o $(RESULTDIR)/ expr.g4
mkmain:
	clang++ -DTRACE -g -std=c++11  -I $(RESULTDIR) -I $(ANTLRRUNTIME)/antlr4-runtime/ *.cpp IR/*.cpp $(RESULTDIR)/*.cpp -o exe $(ANTLRRUNTIME)/lib/libantlr4-runtime.a
	#g++ -DTRACE -g -std=c++11  -I $(RESULTDIR) -I $(ANTLRRUNTIME)/antlr4-runtime/ *.cpp IR/*.cpp $(RESULTDIR)/*.cpp -o exe $(ANTLRRUNTIME)/lib/libantlr4-runtime.a
	

mkexec:
	./exe code.if -a -s test.s
	gcc test.s -o test.exe -no-pie
	./test.exe ; echo $$?


.PHONY: clean test

test: 
	@make -C ./test --no-print-directory
clean:
	find . -type f \( -name "*.test*" -o -name "*.out" -o -name "*.s" -o -name "*.exe" -o -name "*.expected" -o -name "*.tmp" -o -name "*.test" \) -print -delete
