BIN=test net
CFLAGS ?= -pedantic-errors -pipe -Wl,--relax -Wall -Wextra -Wno-variadic-macros -Wno-strict-aliasing -ansi -pedantic -g 
CC = musl-gcc 

all: $(BIN)

dep:
	$(CC) -MM *.c

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean
