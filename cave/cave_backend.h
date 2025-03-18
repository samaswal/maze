#ifndef CAVE_BACKEND_H_
#define CAVE_BACKEND_H_
#include <time.h>
#include <stdlib.h>
#include "../structs.h"
#include "cave_generation.h"

int init_cave_struct(CaveInfo *cave_info, int r, int c);
void destroy_cave_struct(CaveInfo *c_info);
void set_cave_primal(CaveInfo *c_info);
int generate_cave_file(char *filename, int rows, int columns, CaveInfo *c_info);
int write_cave_file(const CaveInfo *c_info, char *fileName);
int init_cave_from_file(CaveInfo *c_info, char *filename);
void cave_set_timer(CaveInfo *c_info);
//void generate_cave(CaveInfo *c_info);

#endif
