#ifndef MAZE_DRAWING_H_
#define MAZE_DRAWING_H_

#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "cave/cave_backend.h"

void draw_single_cell(const MazeInfo *maze_info, int y, int x);
void draw_maze(const MazeInfo *maze_info);
void draw_cursor(int y, int x);
UserInfo *get_user_struct();
void init_user_info(UserInfo *u_info, MazeInfo *m_info);
int pick_point(UserInfo *u_info);
void pick_agent_start_point(UserInfo *u_info);
void draw_picked_points(const UserInfo *u_info);
void draw_track(const MazeInfo *m_info);
void display_modes_menu(int selected);
void display_files_modes_menu(int selected, const char *prompt, const char *var1, const char *var2);
void display_filename_input_menu(const char *prompt, char *filename);
void display_cave_gen_mode_menu(int selected);
void display_size_input_menu(const char *prompt, int row, int *val, int min, int max);
void draw_controls_maze_hint();
void draw_cave(const CaveInfo *cave_info);
void draw_agent(const MLInfo *ml_info);
void mv_curs_left(UserInfo *u_info);
void mv_curs_right(UserInfo *u_info);
void mv_curs_down(UserInfo *u_info);
void mv_curs_up(UserInfo *u_info);


#endif
