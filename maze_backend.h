#ifndef MAZE_BACKEND_H_
#define MAZE_BACKEND_H_
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "structs.h"

MazeInfo *get_maze_struct();

int init_maze_struct(MazeInfo *maze_info, int r, int c);
void destroy_maze_struct(MazeInfo *m_info);
void generate_new_maze(MazeInfo *m_info);
void generate_eller(MazeInfo *m_info);

void print_matrices(MazeInfo *m_info);

#endif
