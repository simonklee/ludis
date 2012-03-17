BIN=ludis-test fd-test str-test addr-test handle-test cw
CFLAGS ?= -std=c89 -pedantic -pedantic-errors -Wl,--relax -Wall -Wextra \
		  -Wno-variadic-macros -Wno-strict-aliasing -D_POSIX_C_SOURCE=200112L
DEBUG ?= -g -ggdb
CC = gcc 
OBJ = addr.o ludis.o lmalloc.o str.o fd.o addr.o handle.o

SRCCOLOR="\033[34m"
BINCOLOR="\033[39;1m"
ENDCOLOR="\033[0m"

COLOR_CC = @printf '    %b   %b\n' CC $(SRCCOLOR)$@$(ENDCOLOR);
COLOR_LINK = @printf '    %b %b\n' LINK $(BINCOLOR)$@$(ENDCOLOR);
COLOR_TEST = @printf '    %b %b\n' $(BINCOLOR)$@$(ENDCOLOR);

all: $(BIN)

addr.o: addr.c addr.h
addr-test.o: addr-test.c addr.h common.h test.h
context.o: context.c context.h
fd.o: fd.c addr.h common.h fd.h
fd-test.o: fd-test.c fd.h addr.h common.h test.h
handle.o: handle.c fd.h addr.h common.h handle.h str.h lmalloc.h
handle-test.o: handle-test.c common.h test.h handle.h addr.h str.h
lmalloc.o: lmalloc.c
ludis.o: ludis.c common.h lmalloc.h test.h ludis.h str.h
ludis-test.o: ludis-test.c common.h ludis.h test.h str.h
query.o: query.c
str.o: str.c common.h lmalloc.h str.h fd.h
str-test.o: str-test.c test.h str.h
test.o: test.c test.h
cw.o: cw.c handle.h lmalloc.h common.h addr.h

cw: cw.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

cw-test: clean cw clean_o
	./cw http://simonklee.org

ludis-test: ludis-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

fd-test: fd-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

addr-test: addr-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

str-test: str-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

handle-test: handle-test.o test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

flags:
	@echo "$(CFLAGS) $(DEBUG)"

test: clean cw ludis-test handle-test fd-test addr-test str-test clean_o
	./addr-test
	./fd-test
	./handle-test
	./ludis-test
	./str-test
	./cw http://simonklee.org

test_val: clean ludis-test fd-test add-test str-test clean_o
	valgrind ./addr-test
	valgrind ./fd-test
	valgrind ./ludis-test
	valgrind ./str-test
	valgrind ./handle-test

%.o: %.c
	$(COLOR_CC)$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

dep:
	$(CC) -MM *.c

clean_o:
	rm -f *.o

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean

