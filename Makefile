CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

all: my_readline

my_readline: my_readline.c
	$(CC) $(CFLAGS) -o my_readline my_readline.c

clean:
	rm -f my_readline
