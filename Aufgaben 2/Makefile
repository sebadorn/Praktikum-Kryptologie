CC = gcc
SRCS = $(wildcard aufgabe*.c)
PROGS = $(patsubst %.c,%.out,$(SRCS))
CRYPTOLIB = ../lib/crypto

all: $(CRYPTOLIB).o $(PROGS)

$(CRYPTOLIB).o: $(CRYPTOLIB).c
	$(CC) -c $(CRYPTOLIB).c -o $(CRYPTOLIB).o

%.out: %.c
	$(CC) -g $< -o $@ $(CRYPTOLIB).o

.PHONY: clean

clean:
	rm -f *.out *.o $(CRYPTOLIB).o