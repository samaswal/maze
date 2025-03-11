all:
	gcc -Wall -Wextra -Werror main.c drawing.c maze_info.c -o maze -lncurses
clean:
	rm -rf main
