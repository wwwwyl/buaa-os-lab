IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

_DEPS = fibo.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

all:
	make main.o
	make fibo.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o
