CC = gcc
CFLAGS = -g -Wall
SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%.out,$(SRCS))

all: $(PROGS)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

.PHONY: clean

clean:
	rm -f *.out *.copytxt