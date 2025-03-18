BUILD_DIR=build


all:
	mkdir -p $(BUILD_DIR)
	gcc -Wall -Wextra -Werror main.c drawing.c maze_backend.c tracking.c cave/cave_backend.c cave/cave_generation.c -o $(BUILD_DIR)/maze.out -lncurses
clean:
	rm -rf $(BUILD_DIR)
