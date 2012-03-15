BIN=ludis-test fd-test str-test addr-test
CFLAGS ?= -ansi -pedantic -pedantic-errors -Wl,--relax -Wall -Wextra \
		  -Wno-variadic-macros -Wno-strict-aliasing
DEBUG ?= -g -ggdb
CC = gcc 
OBJ = ludis.o str.o lmalloc.o fd.o addr.o

SRCCOLOR="\033[34m"
BINCOLOR="\033[39;1m"
ENDCOLOR="\033[0m"

COLOR_CC = @printf '    %b   %b\n' CC $(SRCCOLOR)$@$(ENDCOLOR);
COLOR_LINK = @printf '    %b %b\n' LINK $(BINCOLOR)$@$(ENDCOLOR);
COLOR_TEST = @printf '    %b %b\n' $(BINCOLOR)$@$(ENDCOLOR);

all: $(BIN)

addr.o: addr.c
addr-test.o: addr-test.c addr.h common.h test.h
context.o: context.c
fd.o: fd.c addr.h common.h fd.h
fd-test.o: fd-test.c fd.h addr.h common.h test.h
handle.o: handle.c
lmalloc.o: lmalloc.c
ludis.o: ludis.c common.h lmalloc.h test.h ludis.h str.h
ludis-test.o: ludis-test.c common.h ludis.h test.h str.h
query.o: query.c
str.o: str.c common.h lmalloc.h str.h
str-test.o: str-test.c test.h str.h
test.o: test.c test.h

ludis-test: ludis-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

fd-test: fd-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

addr-test: addr-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

str-test: str-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

flags:
	@echo "$(CFLAGS) $(DEBUG)"

test: clean ludis-test fd-test addr-test str-test clean_o
	./ludis-test
	./str-test
	./addr-test
	./fd-test

test_val: clean ludis-test fd-test add-test str-test clean_o
	valgrind ./ludis-test
	valgrind ./str-test
	valgrind ./addr-test
	valgrind ./fd-test

%.o: %.c
	$(COLOR_CC)$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

dep:
	$(CC) -MM *.c

clean_o:
	rm -f *.o

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean

