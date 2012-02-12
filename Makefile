BIN=ludis-test net-test
CFLAGS ?= -ansi -pedantic -pedantic-errors -Wl,--relax -Wall -Wextra \
		  -Wno-variadic-macros -Wno-strict-aliasing
DEBUG ?= -g -ggdb
CC = musl-gcc 
OBJ = ludis.o net.o

SRCCOLOR="\033[34m"
BINCOLOR="\033[39;1m"
ENDCOLOR="\033[0m"

COLOR_CC = @printf '    %b   %b\n' CC $(SRCCOLOR)$@$(ENDCOLOR);
COLOR_LINK = @printf '    %b %b\n' LINK $(BINCOLOR)$@$(ENDCOLOR);

all: $(BIN)

ludis.o: ludis.c ludis.h common.h net.h
ludis-test.o: ludis-test.c common.h ludis.h net.h test.h
net.o: net.c net.h
net-test.o: net-test.c net.h test.h

ludis-test: ludis-test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

net-test: net-test.o $(OBJ)
	$(COLOR_LINK)$(CC) -o $@ $(LDFLAGS) $(DEBUG) $^

flags:
	@echo "$(CFLAGS) $(DEBUG)"

%.o: %.c
	$(COLOR_CC)$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

dep:
	$(CC) -MM *.c

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean
