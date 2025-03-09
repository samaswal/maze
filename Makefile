all:
	gcc -Wall -Wextra -Werror main.c -o maze -lncurses
clean:
	rm -rf main
