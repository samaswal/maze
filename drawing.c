#include "drawing.h"
#include "structs.h"
#include <ncurses.h>

void draw_cursor(int y, int x) {
  //clear();
  chtype cur = ACS_DIAMOND | A_BLINK;
  mvaddch(BORDER_TOP + y, BORDER_LEFT + x, cur);
}

UserInfo *get_user_struct() {
  static UserInfo us;
  return &us;
}

void init_user_info(UserInfo *u_info, MazeInfo *m_info) {
  u_info->y = 1;
  u_info->x = 1;
  u_info->maxY = m_info->rows * 2 - 1;
  u_info->maxX = m_info->columns * 2 - 1;
  u_info->fPickedY = 0;
  u_info->fPickedX = 0;
  u_info->sPickedY = 0;
  u_info->sPickedX = 0;
}

int pick_point(UserInfo *u_info) {
  int res = 0; //is point picked for tracking
  if(u_info) {
    int x = u_info->x;
    int y = u_info->y;
    if(u_info->fPickedX == 0) {
      u_info->fPickedX = x;
      u_info->fPickedY = y;
      //move(2, 2);
      //printw("%d", res);
    }
    else if(u_info->sPickedX == 0) {
      u_info->sPickedX = x;
      u_info->sPickedY = y;
      res = 1;
      move(2, 2);
      printw("%d", res);
    }
    else {
      u_info->fPickedX = 0;
      u_info->fPickedY = 0;
      u_info->sPickedX = 0;
      u_info->sPickedY = 0;
    }
  }
  return res;
}

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

void draw_controls_maze_hint() {
  mvprintw(0, 0, "Pick point");
  mvprintw(1, 0, "Menu");
  mvprintw(2, 0, "Exit");
  attron(A_REVERSE);
  mvprintw(0, 11, "ENTER");
  mvprintw(1, 6, "B");
  mvprintw(2, 6, "Q");
  attroff(A_REVERSE);
}

void draw_maze(MazeInfo *maze_info) {
    if(is_correct_maze(maze_info)) {
        clear();
        draw_controls_maze_hint();
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

void draw_picked_points(UserInfo *u_info) {
  if(u_info) {
    int x1 = u_info->fPickedX;
    int y1 = u_info->fPickedY;
    int x2 = u_info->sPickedX;
    int y2 = u_info->sPickedY;
    chtype start_pointer = 'o' | A_BLINK;
    chtype end_pointer = 'X' | A_BLINK;
    if(has_colors()) {
      start_pointer = start_pointer | COLOR_PAIR(1);
      end_pointer = end_pointer | COLOR_PAIR(1);
    }
    if(y1 > 0 && x1 > 0)
      mvaddch(BORDER_TOP + y1, BORDER_LEFT + x1, start_pointer);
    if(y2 > 0 && x2 > 0)
      mvaddch(BORDER_TOP + y2, BORDER_LEFT + x2, end_pointer);
  }
}

void draw_track(MazeInfo *m_info) {
  int r = m_info->rows;
  int c = m_info->columns;
  chtype ch = ACS_BULLET | A_BLINK;
  chtype vert = ACS_VLINE | A_BLINK;
  chtype hor = ACS_HLINE | A_BLINK;
  if(has_colors()) {
    ch = ch | COLOR_PAIR(1);
    vert = vert | COLOR_PAIR(1);
    hor = hor | COLOR_PAIR(1);
  }
  for(int i = 0; i < r; i++) {
    for(int j = 0; j < c; j++) {
      if(m_info->track_matrix[i][j] <= -1) {
        mvaddch(BORDER_TOP + 1 + (i * 2), BORDER_LEFT + 1 + ((j * 2)), ch);
        if(i + 1 < r && (m_info->track_matrix[i + 1][j] == m_info->track_matrix[i][j] - 1))
          mvaddch(BORDER_TOP + 1 + (i * 2) + 1, BORDER_LEFT + 1 + ((j * 2)), vert);
        if(i - 1 >= 0 && (m_info->track_matrix[i - 1][j] == m_info->track_matrix[i][j] - 1))
          mvaddch(BORDER_TOP + 1 + (i * 2) - 1, BORDER_LEFT + 1 + ((j * 2)), vert);
        if(j + 1 < c && (m_info->track_matrix[i][j + 1] == m_info->track_matrix[i][j] - 1))
          mvaddch(BORDER_TOP + 1 + (i * 2), BORDER_LEFT + 1 + ((j * 2) + 1), hor);
        if(j - 1 >= 0 && (m_info->track_matrix[i][j - 1] == m_info->track_matrix[i][j] - 1))
          mvaddch(BORDER_TOP + 1 + (i * 2), BORDER_LEFT + 1 + ((j * 2) - 1), hor);

      }
    }
  }
}

void display_modes_menu(int selected) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);  // Get screen dimensions

  int width = 30, height = 8;  // Set the dialog box size
  int start_y = (max_y - height) / 2;  // Center the window vertically
  int start_x = (max_x - width) / 2;   // Center the window horizontally

  WINDOW *menu_win = newwin(height, width, start_y, start_x);
  box(menu_win, 0, 0);  // Draw a border around the window

  mvwprintw(menu_win, 1, 1, "Choose mode:");

  const char *colors[] = {"Maze", "Cave", "MazeML", "Exit"};
  for (int i = 0; i < 4; i++) {
    if (i == selected) {
      wattron(menu_win, A_REVERSE); // Invert colors for the selected item
    }
    mvwprintw(menu_win, i + 2, 1, "%s", colors[i]);
    if (i == selected) {
      wattroff(menu_win, A_REVERSE); // Turn off color inversion
    }
  }

  wrefresh(menu_win);  // Refresh the window to show content
  delwin(menu_win);
}

void display_files_modes_menu(int selected) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  int width = 30, height = 8;
  int start_y = (max_y - height) / 2;
  int start_x = (max_x - width) / 2;
  WINDOW *menu_win = newwin(height, width, start_y, start_x);
  box(menu_win, 0, 0);
  mvwprintw(menu_win, 1, 1, "Source:");
  const char *modes[] = {"Generate file", "Choose file", "Back", "Exit"};
  for (int i = 0; i < 4; i++) {
    if (i == selected) {
      wattron(menu_win, A_REVERSE); // Invert colors for the selected item
    }
    mvwprintw(menu_win, i + 2, 1, "%s", modes[i]);
    if (i == selected) {
      wattroff(menu_win, A_REVERSE); // Turn off color inversion
    }
  }
  wrefresh(menu_win);  // Refresh the window to show content
  delwin(menu_win);
}

void display_filename_input_menu(const char *prompt, char *filename) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);  // Get screen dimensions

  int width = 30, height = 5;  // Set the dialog box size for input
  int start_y = (max_y - height) / 2;  // Center the window vertically
  int start_x = (max_x - width) / 2;   // Center the window horizontally

  WINDOW *input_win = newwin(height, width, start_y, start_x);
  box(input_win, 0, 0);  // Draw a border around the window

  mvwprintw(input_win, 1, 1, prompt);
  wrefresh(input_win);

  echo();
  mvwgetnstr(input_win, 3, 1, filename, 20);  // Read a string from the user
  noecho();
  delwin(input_win);
}



void display_size_input_menu(const char *prompt, int row, int *val) {
  char num[4];
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  int width = 30, height = 5;
  int start_y = (max_y - height) / 2;
  int start_x = (max_x - width) / 2;

  WINDOW *input_win = newwin(height, width, start_y, start_x);
  box(input_win, 0, 0);
  if(*prompt != '\0')
    mvwprintw(input_win, 1, 1, prompt);
  wrefresh(input_win);
  echo();
  mvwgetnstr(input_win, row, 1, num, 20);
  noecho();
  *val = atoi(num);
  while(*val < 4 || *val > 50) {
    mvwprintw(stdscr, 1, 1, "Incorrect size! Size should be from 4 to 50!");
    refresh();
    echo();
    mvwgetnstr(input_win, row, 1, num, 20);
    noecho();
    *val = atoi(num);
  }
  delwin(input_win);
}

void draw_cave(CaveInfo *cave_info) {
  if(is_correct_cave(cave_info)) {
    clear();
    draw_controls_maze_hint();
    int r = cave_info->rows;
    int c = cave_info->columns;
/*
    mvaddch(BORDER_TOP, BORDER_LEFT, ACS_ULCORNER);
    mvaddch(BORDER_TOP, BORDER_LEFT + c * 2, ACS_URCORNER);
    mvaddch(BORDER_TOP + r * 2, BORDER_LEFT, ACS_LLCORNER);*/
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        if(cave_info->matrix[i][j])
          mvaddch(BORDER_TOP + i, BORDER_LEFT + j, ACS_BOARD);
      }
    }
  }
}
