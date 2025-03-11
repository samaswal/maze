#include "drawing.h"

void draw_single_cell(MazeInfo *maze_info, int y, int x) {
  if(maze_info->matrix1[y][x]) {
    if(y == 0 && x + 1 < maze_info->columns) mvaddch(BORDER_TOP + (y * 2), BORDER_LEFT + 1 + ((x * 2) + 1), ACS_TTEE);
    mvaddch(BORDER_TOP + 1 + (y * 2), BORDER_LEFT + 1 + ((x * 2) + 1), ACS_VLINE);
  }
  if(maze_info->matrix2[y][x]) {
    if(x == 0 && y + 1 < maze_info->rows)
      mvaddch(BORDER_TOP + 1 + ((y * 2) + 1), BORDER_LEFT  + (x * 2), ACS_LTEE);
    mvaddch(BORDER_TOP + 1 + ((y * 2) + 1), BORDER_LEFT + 1 + (x * 2), ACS_HLINE);
  }
  if(maze_info->matrix1[y][x] && maze_info->matrix2[y][x]) {
    mvaddch(BORDER_TOP + 1 + ((y * 2) + 1), BORDER_LEFT + 1 + ((x * 2) + 1), ACS_LRCORNER);
  }
  if(x + 1 < maze_info->columns && (maze_info->matrix1[y][x] && maze_info->matrix2[y][x] && maze_info->matrix2[y][x + 1]))
    mvaddch(BORDER_TOP + 1 + ((y * 2) + 1), BORDER_LEFT + 1 + ((x * 2) + 1), ACS_BTEE);
  if((x + 1 < maze_info->columns) && maze_info->matrix1[y][x] && !maze_info->matrix2[y][x] && maze_info->matrix2[y][x + 1])
    mvaddch(BORDER_TOP + 1 + ((y * 2) + 1), BORDER_LEFT + 1 + ((x * 2) + 1), ACS_LLCORNER);
  if((y + 1 < maze_info->rows) && maze_info->matrix1[y][x] && maze_info->matrix1[y + 1][x] && !(maze_info->matrix1[y][x] && maze_info->matrix2[y][x])) {
      mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_VLINE);
  }
  if((y + 1 < maze_info->rows) && maze_info->matrix2[y][x] && maze_info->matrix1[y + 1][x]) {
    if(x + 1 < maze_info->columns && maze_info->matrix2[y][x + 1]) {
      mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_TTEE);
    }
    else
      mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_URCORNER);
  }
  if((y - 1 >= 0) && (x + 1 < maze_info->columns) && maze_info->matrix1[y][x] && maze_info->matrix2[y - 1][x + 1]) {
    if(maze_info->matrix1[y - 1][x]) {
      if(maze_info->matrix2[y - 1][x]) {
        mvaddch(BORDER_TOP + 1 + (y * 2) - 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_PLUS);
      }
      else
        mvaddch(BORDER_TOP + 1 + (y * 2) - 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_LTEE);
    }
    else if(maze_info->matrix2[y - 1][x]) {
      if(maze_info->matrix1[y - 1][x]) {
        mvaddch(BORDER_TOP + 1 + (y * 2) - 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_PLUS);
      }
      else
        mvaddch(BORDER_TOP + 1 + (y * 2) - 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_TTEE);
    }
    else
      mvaddch(BORDER_TOP + 1 + (y * 2) - 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_ULCORNER);
  }
  if((y + 1 < maze_info->rows) && maze_info->matrix1[y][x] && maze_info->matrix1[y + 1][x] && (maze_info->matrix1[y][x] && maze_info->matrix2[y][x])) {
    if(x + 1 <maze_info->columns && maze_info->matrix2[y][x + 1]) {
      mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_PLUS);
    }
    else
      mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + ((x * 2) + 1), ACS_RTEE);
    }
  if((x + 1 < maze_info->columns) && maze_info->matrix2[y][x] && maze_info->matrix2[y][x + 1] && !(maze_info->matrix1[y][x] && maze_info->matrix2[y][x]))
    mvaddch(BORDER_TOP + 1 + (y * 2) + 1, BORDER_LEFT + 1 + (x * 2) + 1, ACS_HLINE);
}

void draw_maze(MazeInfo *maze_info) {
    if(is_correct_maze(maze_info)) {
        int r = maze_info->rows;
        int c = maze_info->columns;
        mvhline(BORDER_TOP, BORDER_LEFT, 0, c * 2);
        mvvline(BORDER_TOP, BORDER_LEFT, 0, r * 2);

        mvaddch(BORDER_TOP, BORDER_LEFT, ACS_ULCORNER);
        mvaddch(BORDER_TOP, BORDER_LEFT + c * 2, ACS_URCORNER);
        mvaddch(BORDER_TOP + r * 2, BORDER_LEFT, ACS_LLCORNER);
        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                draw_single_cell(maze_info, i, j);
            }
        }
    }
}
