BIN=ludis-test net-test str-test
CFLAGS ?= -ansi -pedantic -pedantic-errors -Wl,--relax -Wall -Wextra \
		  -Wno-variadic-macros -Wno-strict-aliasing
DEBUG ?= -g -ggdb
CC = gcc 
OBJ = ludis.o net.o str.o lmalloc.o

SRCCOLOR="\033[34m"
BINCOLOR="\033[39;1m"
ENDCOLOR="\033[0m"

COLOR_CC = @printf '    %b   %b\n' CC $(SRCCOLOR)$@$(ENDCOLOR);
COLOR_LINK = @printf '    %b %b\n' LINK $(BINCOLOR)$@$(ENDCOLOR);
COLOR_TEST = @printf '    %b %b\n' $(BINCOLOR)$@$(ENDCOLOR);

all: $(BIN)

addr.o: addr.c
lmalloc.o: lmalloc.c
ludis.o: ludis.c common.h ludis.h
ludis-test.o: ludis-test.c common.h ludis.h test.h
net.o: net.c common.h net.h
net-test.o: net-test.c net.h common.h test.h
query.o: query.c
str.o: str.c str.h common.h lmalloc.h
str-test.o: str-test.c test.h str.h
test.o: test.c test.h

ludis-test: ludis-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

net-test: net-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

str-test: str-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

flags:
	@echo "$(CFLAGS) $(DEBUG)"

test: clean ludis-test net-test str-test clean_o
	./ludis-test
	./net-test
	./str-test

test_val: clean ludis-test net-test str-test clean_o
	valgrind ./ludis-test
	valgrind ./net-test
	valgrind ./str-test

%.o: %.c
	$(COLOR_CC)$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

dep:
	$(CC) -MM *.c

clean_o:
	rm -f *.o

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean
