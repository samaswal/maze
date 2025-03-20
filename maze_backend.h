#ifndef MAZE_BACKEND_H_
#define MAZE_BACKEND_H_
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "structs.h"

int init_maze_struct(MazeInfo *maze_info, int r, int c);
void destroy_maze_struct(MazeInfo *m_info);
void generate_new_maze(MazeInfo *m_info);
void generate_eller(MazeInfo *m_info);
int is_correct_maze(const MazeInfo *m_info);
void print_matrices(MazeInfo *m_info);
void unite_set(int *set, int id, int columns);
int is_horizontable(int index, int *line, int *set, int columns);
int generate_maze_file(char *fileName, int rows, int columns, MazeInfo *m_info);

#endif
