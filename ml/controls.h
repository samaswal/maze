#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "../structs.h"
#include "save_load.h"

typedef struct Flags {
  int loadfile;
  int generate;
  int train;
  int learn;
} Flags;

int get_flags_input(int argc, char *argv[], MLInfo *ml_info, Flags *flags, char *filename);
int handle_maze_ml_input(int c, UserInfo *u_info, MLInfo *ml_info);
void set_start_pos(MLInfo *ml_info, int y, int x);


#endif
