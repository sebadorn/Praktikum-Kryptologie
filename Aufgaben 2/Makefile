CC = gcc
CFLAGS = -g
SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%.out,$(SRCS))

all: $(PROGS)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.out