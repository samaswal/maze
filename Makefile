BUILD_DIR=build
EXEC_FILE=maze.out
CC=gcc
CCFLAGS=-Wall -Wextra -Werror --std=c11


all: install run
	
main.o:
	$(CC) $(CCFLAGS) -c main.c -o main.o
	
drawing.o:
	$(CC) $(CCFLAGS) -c drawing.c -o drawing.o
	
maze_backend.o:
	$(CC) $(CCFLAGS) -c maze_backend.c -o maze_backend.o
	
tracking.o:
	$(CC) $(CCFLAGS) -c tracking.c -o tracking.o

cave_backend.o:
	$(CC) $(CCFLAGS) -c cave/cave_backend.c -o cave_backend.o

cave_generation.o:
	$(CC) $(CCFLAGS) -c cave/cave_generation.c -o cave_generation.o

	
install: build_dir main.o drawing.o maze_backend.o tracking.o cave_backend.o cave_generation.o
	$(CC) $(CCFLAGS) main.o drawing.o maze_backend.o tracking.o cave_backend.o cave_generation.o -o $(BUILD_DIR)/$(EXEC_FILE) -lncurses

run:
	cd $(BUILD_DIR) && ./$(EXEC_FILE)

build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf *.o

uninstall:
	rm -rf $(BUILD_DIR)

.PHONY: all install run clean uninstall build_dir
