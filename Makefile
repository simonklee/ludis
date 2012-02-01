BIN=ludis
CFLAGS ?= -pedantic-errors -pipe -Wl,--relax -Wall -Wextra -Wno-variadic-macros -Wno-strict-aliasing -ansi -pedantic -g -DTEST
CC = musl-gcc 

all: $(BIN)

%.o: %.c
	@echo '		CC $@'
	@$(CC) $(CFLAGS) -c $< -o $@

%.out: %.o 
	@$(CC) $(LDFLAGS) $^ -o $@

dep:
	$(CC) -MM *.c

clean:
	rm -f $(BIN) *.o

.PHONY: all dep clean
