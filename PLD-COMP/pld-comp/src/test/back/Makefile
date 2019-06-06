all-tests := $(addsuffix .test, $(basename $(wildcard */*.c)))


default: $(all-tests)
	@echo "back end tests done"

%.test : %.c
	@../../exe $(@:%.test=%.c) -a -s $(@:%.test=%.s) > /dev/null 2> /dev/null
	@gcc $(@:%.test=%.s) -o tmp.exe > /dev/null 2> /dev/null
	@./tmp.exe ; echo $$? > $(@:%.test=%.out)
	@gcc $(@:%.test=%.c) -o tmp.exe -no-pie
	@./tmp.exe ; echo $$? > $(@:%.test=%.expected)
	@diff -q $(@:%.test=%.expected) $(@:%.test=%.out) || ( echo "Test $@ failed" ; false )
	

clean:
	rm -rf *.tmp
