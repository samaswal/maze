all:
	gcc -Wall -Wextra -Werror main.c drawing.c maze_backend.c tracking.c cave/cave_backend.c -o maze.out -lncurses
clean:
	rm -rf maze.out
