TESTS=test-test
OBJ=

all: $(TESTS)

test-%: test-%.o $(OBJ) 
	$(CC) -o $@ $^

.c.o:
	$(CC) -std=c99 -pedantic -c -O0 $<

clean:
	rm -f *.o $(TESTS)

dep:
	$(CC) -MM *.c > Makefile.dep

.PHONY: all clean dep
