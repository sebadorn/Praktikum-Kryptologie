CC = gcc
NAME = crypto

all: $(NAME).o

$(NAME).o: $(NAME).c
	$(CC) -c -Wall $(NAME).c -o $(NAME).o

.PHONY: clean

clean:
	rm -f $(NAME).o