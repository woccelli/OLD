all-tests := $(addsuffix .test, $(basename $(wildcard */*.c)))

default: $(all-tests)
	@echo "front end tests done"

%.test : %.c
	@../../exe $(@:%.test=%.c) -a -s $(@:%.test=%.s) > /dev/null 2> /dev/null; \
            if [ $$? -eq 0 ]; then \
                false; \
            fi

clean:
	rm -rf *.s
