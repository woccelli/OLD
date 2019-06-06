all-tests := $(addsuffix .test, $(basename $(wildcard *.c)))


default: $(all-tests)
	@echo "putchar tests done"

%.test : %.c
	@../../exe $(@:%.test=%.c) -a -s $(@:%.test=%.s) > /dev/null 2> /dev/null
	@gcc $(@:%.test=%.s) -o tmp.exe -no-pie > /dev/null  2> /dev/null
	@./tmp.exe > $(@:%.test=%.out) 2> /dev/null
	@gcc $(@:%.test=%.c) -o tmp.exe -no-pie > /dev/null  2> /dev/null
	@./tmp.exe > $(@:%.test=%.expected)
	@diff -q $(@:%.test=%.expected) $(@:%.test=%.out) || ( echo "Test $@ failed" ; false )
	

clean:
	rm -rf *.out *.expected *.s