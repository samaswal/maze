all:
	gcc -Wall -Wextra -Werror main.c drawing.c maze_backend.c -o maze -lncurses
clean:
	rm -rf maze
