#include <ncurses.h>
#include <stdio.h>
#include "maze_info.h"

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
        mvhline(BORDER_TOP, BORDER_LEFT, 0, r * 2);
        mvvline(BORDER_TOP, BORDER_LEFT, 0, c * 2);

        mvaddch(BORDER_TOP, BORDER_LEFT, ACS_ULCORNER);
        mvaddch(BORDER_TOP, BORDER_LEFT + c * 2, ACS_URCORNER);
        mvaddch(BORDER_TOP + r * 2, BORDER_LEFT, ACS_LLCORNER);

        /*mvvline(BORDER_TOP + 1, BORDER_LEFT + 1, 0, 1);

        mvaddch(BORDER_LEFT + r * 3, BORDER_LEFT + c * 3, ACS_LRCORNER);
       */
      for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
          draw_single_cell(maze_info, i, j);
        }
      }
    }
}

int get_user_input(int argc, char *argv[], MazeInfo *m_info) {
  int res = OK;
  int rows = 0, columns = 0;
  if(argc == 3) {
    if(sscanf(argv[1], "%d", &rows) != 1 || rows < 0) {
      res = INPUT_ERR;
    }
    if(sscanf(argv[2], "%d", &columns) != 1 || rows < 0) {
      res = INPUT_ERR;
    }
  }
  else res = INPUT_ERR;
  if(res != INPUT_ERR) init_maze_struct(m_info, rows, columns);
  return res;
}

int init_maze_from_file(MazeInfo *m_info, int argc, char *argv[]) {
  int res = OK;
  FILE *f;
  int r, c;
  if(argc == 2) {
    f = fopen(argv[1], "r");
    if(!f) res = INPUT_ERR;
  }
  else res = INPUT_ERR;
  if(res == OK) {
    if(fscanf(f, "%d", &r) != 1 || r < 0 || r > 50) res = INPUT_ERR;
    if(fscanf(f, "%d", &c) != 1 || c < 0 || c > 50) res = INPUT_ERR;
  }
  if(res == OK) {
    res = init_maze_struct(m_info, r, c);
  }
  if(res == OK) {
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        int sc = fscanf(f, "%d", &m_info->matrix1[i][j]);
        if(sc != 1) {
          res = INPUT_ERR;
          break;
        }
      }
      if(res == INPUT_ERR) break;
    }
  }
  if(res == OK) {
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        int sc = fscanf(f, "%d", &m_info->matrix2[i][j]);
        if(sc != 1) {
          res = INPUT_ERR;
          break;
        }
      }
      if(res == INPUT_ERR) break;
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  MazeInfo *maze_info = get_maze_struct();
  int flag = 1;
  int is_ok = OK;
  //flag = get_user_input(argc, argv, maze_info);
  is_ok = init_maze_from_file(maze_info, argc, argv);
  if(is_ok != OK) destroy_maze_struct(maze_info);
  if(is_ok == OK) flag = 1;
  else flag = 0;
  //print_matrices(maze_info);
 if(is_ok == OK) {
  initscr();
  noecho();
  curs_set(0);

  while(flag) {
    draw_maze(maze_info);
    if(getch() == 'p') flag = 0;
    timeout(500);
  }
}

  destroy_maze_struct(maze_info);	
  endwin();
}
