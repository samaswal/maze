#ifndef MAZE_BACKEND_H_
#define MAZE_BACKEND_H_
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "structs.h"

#define BORDER_TOP 5
#define BORDER_LEFT 5
#define MALLOC_ERR -1
#define INPUT_ERR 1
#define HELP_MODE 2

MazeInfo *get_maze_struct();

int init_maze_struct(MazeInfo *maze_info, int r, int c);
void destroy_maze_struct(MazeInfo *m_info);
void generate_new_maze(MazeInfo *m_info);
void generate_eller(MazeInfo *m_info);

void print_matrices(MazeInfo *m_info);

#endif
