#ifndef TRACKING_H_
#define TRACKING_H_
#include "structs.h"

void track(MazeInfo *m_info, UserInfo *u_info);
int is_unobstructed(MazeInfo *m_info, int y1, int x1, int y2, int x2);
#endif
