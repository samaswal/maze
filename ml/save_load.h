#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_
#include "qlearn.h"
#include "../maze_backend.h"
#include "../drawing.h"

int init_ml_maze_from_file(MLInfo *ml_info, char *filename);
int create_maze_ml_struct(MLInfo *ml_info, int r, int c, int y, int x);
int write_maze_ml_file(MLInfo *ml_info, char *fileName);
int generate_maze_ml_file(char *fileName, int rows, int columns, int y, int x, MLInfo *ml_info);
void destroy_maze_ml_struct(MLInfo *ml_info);
void q_random_init(MLInfo *ml_info);
void save_q_table(MLInfo *ml_info, char *fileName);

#endif
