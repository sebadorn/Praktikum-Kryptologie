CC = gcc
SRCS = $(wildcard aufgabe*.c)
PROGS = $(patsubst %.c,%.out,$(SRCS))
CRYPTOLIB = ../lib/crypto

all: $(PROGS)

cryptolib: $(CRYPTOLIB).c
	$(CC) -c -Wall $(CRYPTOLIB).c -o $(CRYPTOLIB).o

%.out: %.c
	$(CC) -g -lm -Wall $< -o $@ $(CRYPTOLIB).o

.PHONY: clean

clean:
	rm -f *.out *.o

.PHONY: cleancryptolib

cleancryptolib:
	rm -f $(CRYPTOLIB).o