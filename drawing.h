#include "maze_backend.h"

typedef struct UserInfo {
  int y;
  int x;
  int maxY;
  int maxX;
  int fPickedX;
  int fPickedY;
  int sPickedX;
  int sPickedY;
} UserInfo;

void draw_single_cell(MazeInfo *maze_info, int y, int x);
void draw_maze(MazeInfo *maze_info);
void draw_cursor(int y, int x);
UserInfo *get_user_struct();
void init_user_info(UserInfo *u_info, MazeInfo *m_info);
void pick_point(UserInfo *u_info);
void draw_picked_points(UserInfo *u_info);
