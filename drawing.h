#ifndef MAZE_DRAWING_H_
#define MAZE_DRAWING_H_

#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "cave/cave_backend.h"

void draw_single_cell(MazeInfo *maze_info, int y, int x);
void draw_maze(MazeInfo *maze_info);
void draw_cursor(int y, int x);
UserInfo *get_user_struct();
void init_user_info(UserInfo *u_info, MazeInfo *m_info);
int pick_point(UserInfo *u_info);
void draw_picked_points(UserInfo *u_info);
void draw_track(MazeInfo *m_info);
void display_modes_menu(int selected);
void display_files_modes_menu(int selected);
void display_filename_input_menu(const char *prompt, char *filename);
void display_cave_gen_mode_menu(int selected);
void display_size_input_menu(const char *prompt, int row, int *val, int min, int max);
void draw_controls_maze_hint();
void draw_cave(CaveInfo *cave_info);

#endif
