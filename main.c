#include "cave/cave_backend.h"
#include "drawing.h"
#include "maze_backend.h"
#include "structs.h"
#include "tracking.h"

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

void print_help() {
  printf("Usage:\n"
         "-p - Set points mode\n"
         "-y <int 0 to 49> - point y pos\n"
         "-x <int 0 to 49> - point x pos\n"
         "-f <filename> - open file\n"
         "-g <filename> -r <int 4 to 50> -c <int 4 to 50> or - generate maze file\n"
         "-r <int 4 to 50> - new maze rows number\n"
         "-c <int 4 to 50> - new maze columns number\n");
}

void print_error(int code) {
  switch (code) {
    case MALLOC_ERR:
      perror("Malloc error");
      break;
    case INPUT_ERR:
      perror("Incorrect input");
      break;
  }
}

int init_maze_from_file(MazeInfo *m_info, char *fileName) {
  int res = OK;
  FILE *f;
  int r, c;
  f = fopen(fileName, "r");
  if(!f) res = INPUT_ERR;
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
  if(f) fclose(f);
  return res;
}

int write_maze_file(MazeInfo *m_info, char *fileName) {
  int res = OK;
  FILE *f = fopen(fileName, "w");
  if(!f) res = INPUT_ERR;
  else {
    fprintf(f, "%d %d\n", m_info->rows, m_info->columns);
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns; j++) {
        fprintf(f, "%d ", m_info->matrix1[i][j]);
      }
      fprintf(f, "\n");
    }
    fprintf(f, "\n");
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns; j++) {
        fprintf(f, "%d ", m_info->matrix2[i][j]);
      }
      fprintf(f, "\n");
    }
  }
  if(f) fclose(f);
  return res;
}

int generate_maze_file(char *fileName, int rows, int columns, MazeInfo *m_info) {
  int res = init_maze_struct(m_info, rows, columns);
  if(res == OK) {
    generate_eller(m_info);
    res = write_maze_file(m_info, fileName);
  }
  return res;
}

int get_user_input(int argc, char *argv[], MazeInfo *m_info) {
  int res = OK;
  int current_flag = getopt(argc, argv, "pg:f:y:x:");
  for(; current_flag != -1 && res == OK; current_flag = getopt(argc, argv, "pg:f:y:x:")) {
    switch (current_flag) {
      case 'f':
        if(res == OK)
          res = init_maze_from_file(m_info, optarg);
        break;
      case 'h':
        res = HELP;
        print_help();
        break;
      case 'g':
        int row = 0, col = 0;
        if(res == OK) {
          printf("Enter new maze rows number:\n");
          char nm[10];
          fgets(nm, 10, stdin);
          row = atoi(nm);
          while(row < 4 || row > 50) {
            printf("Rows number should be from 4 to 50!\n");
            fgets(nm, 10, stdin);
            row = atoi(nm);
          }
          printf("Enter new maze columns number:\n");
          fgets(nm, 10, stdin);
          col = atoi(nm);
          while(col < 4 || col > 50) {
            printf("Columns number should be from 4 to 50!\n");
            fgets(nm, 10, stdin);
            col = atoi(nm);
          }
          res = generate_maze_file(optarg, row, col, m_info);
        }
        break;
      default:
        res = INPUT_ERR;
        print_help();
        break;
    }
  }
  return res;
}

void mv_curs_left(UserInfo *u_info) {
  if(u_info->x > 1) u_info->x -= 2;
}

void mv_curs_right(UserInfo *u_info) {
  if(u_info->x < u_info->maxX) u_info->x += 2;
}

void mv_curs_down(UserInfo *u_info) {
  if(u_info->y < u_info->maxY) u_info->y += 2;
}

void mv_curs_up(UserInfo *u_info) {
  if(u_info->y > 1) u_info->y -= 2;
}

int handle_maze_input(int c, UserInfo *u_info, MazeInfo *m_info) {
  int res = OK;
  switch (c) {
    case 'q':
      res = EXIT;
      break;
    case 'Q':
      res = EXIT;
      break;
    case 'b':
      res = BACK;
      break;
    case 'B':
      res = BACK;
      break;
    case KEY_LEFT:
      mv_curs_left(u_info);
      break;
    case KEY_RIGHT:
      mv_curs_right(u_info);
      break;
    case KEY_DOWN:
      mv_curs_down(u_info);
      break;
    case KEY_UP:
      mv_curs_up(u_info);
      break;
    case 10:
      if(pick_point(u_info)) track(m_info, u_info);
      else clear_matr(m_info->track_matrix, m_info->rows, m_info->columns);
      break;
    default:
      break;
  }
  return res;
}

int handle_cave_input(int c, CaveInfo *c_info) {
  int res = OK;
  switch (c) {
    case 'q':
      res = EXIT;
      break;
    case 'b':
      res = BACK;
      break;
    case 'Q':
      res = EXIT;
      break;
    case 'B':
      res = BACK;
      break;
    case 10:
      if(c_info->gen_mode == STEP_BY_STEP) {
        generate_cave(c_info);
      }
  }
  return res;
}

int mode_chosing(int *mode) {
  int res = OK, key = 0;
  display_modes_menu(0);
  refresh();
  while(res == OK) {
    display_modes_menu(*mode);

    key = getch();
    if (key == KEY_UP && *mode > 0) {
      (*mode)--;
    } else if (key == KEY_DOWN && *mode < 3) {
      (*mode)++;
    } else if (key == 10) {
      if(*mode == 3) res = EXIT;
      break;
    }
  }
  return res;
}

int file_mode_chosing(int *mode) {
  int res = OK, key = 0;
  display_files_modes_menu(0);
  refresh();
  while(res == OK) {
    display_files_modes_menu(*mode);
    key = getch();
    if (key == KEY_UP && *mode > 0) {
      (*mode)--;
    } else if (key == KEY_DOWN && *mode < 4) {
      (*mode)++;
    } else if (key == 10) {
      if(*mode == 3) res = EXIT;
      else if(*mode == 2) res = BACK;
      break;
    }
  }
  return res;
}

void cave_gen_mode_choosing(int *genmode) {
  int res = OK, key = 0;
  display_cave_gen_mode_menu(0);
  refresh();
  while(res == OK) {
    display_cave_gen_mode_menu(*genmode);
    key = getch();
    if (key == KEY_UP && *genmode > 0) {
      *genmode = 0;
    } else if (key == KEY_DOWN && *genmode < 2) {
      *genmode = 1;
    } else if (key == 10) {
      res = EXIT;
    }
  }
}

int maze_output_from_file() {
  int res = OK;
  char filename[40];
  MazeInfo maze_info;
  UserInfo user_info;
  while (res == OK) {
    display_filename_input_menu("Filename:", filename);
    res = init_maze_from_file(&maze_info, filename);
    if(res == OK) init_user_info(&user_info, &maze_info);
    clear();
    while (res == OK) {
      draw_maze(&maze_info);
      draw_cursor(user_info.y, user_info.x);
      draw_track(&maze_info);
      draw_picked_points(&user_info);
      res = handle_maze_input(getch(), &user_info, &maze_info);
      refresh();
    }
    clear();
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_struct(&maze_info);
  }
  return res;
}

int cave_display_state(CaveInfo *c_info) {
  int res = OK;
  while(res == OK) {
    draw_cave(c_info);
    res = handle_cave_input(getch(), c_info);
    timeout(1);
    generate_automatic(c_info);
    refresh();
  }
  clear();
  return res;
}

int cave_open_file_mode_picked(int mode) {
  int res = OK;
  CaveInfo cave_info;
  char filename[40];
  int genmode = 0;
  if(mode == 1) { //open file mode
    display_filename_input_menu("Filename:", filename);
    res = init_cave_from_file(&cave_info, filename);
    clear();
  }
  else if(mode == 0) { //generate file mode
    int row = 0, col = 0;
    display_size_input_menu("Rows:", 2, &row, 4, 50);
    display_size_input_menu("Columns:", 2, &col, 4, 50);
    display_filename_input_menu("Filename:", filename);
    res = generate_cave_file(filename, row, col, &cave_info);
    clear();
  }
  if(res == OK) {
    display_size_input_menu("Birth limit:", 2, &cave_info.birth_limit, 0, 7);
    display_size_input_menu("Death limit:", 2, &cave_info.death_limit, 0, 7);
    clear();
    cave_gen_mode_choosing(&genmode);
    cave_info.gen_mode = (genmode == 0) ? AUTOMATIC : STEP_BY_STEP;
    if(genmode == 0) {
      clear();
      int tm = 0;
      display_size_input_menu("Gen Time Delay:", 2, &tm, 10, 1000);
      cave_info.time_delay = tm;
    }
    clear();
    res = cave_display_state(&cave_info);
  }
  clear();
  if(res != INPUT_ERR && res != MALLOC_ERR) destroy_cave_struct(&cave_info);
  return res;
}

int maze_generation_mode_picked() {
  int res = OK;
  MazeInfo maze_info;
  UserInfo user_info;
  while(res == OK) {
    int row = 0, col = 0;
    char filename[40];
    display_size_input_menu("Rows:", 2, &row, 4, 50);
    display_size_input_menu("Columns:", 3, &col, 4, 50);
    display_filename_input_menu("Filename:", filename);
    res = generate_maze_file(filename, row, col, &maze_info);
    if(res == OK) init_user_info(&user_info, &maze_info);
    clear();
    while (res == OK) {
      draw_maze(&maze_info);
      draw_cursor(user_info.y, user_info.x);
      draw_track(&maze_info);
      draw_picked_points(&user_info);
      res = handle_maze_input(getch(), &user_info, &maze_info);
      refresh();
    }
    clear();
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_struct(&maze_info);
  }
  return res;
}

int process_maze_mode() {
  clear();
  int res = OK, maze_mode = 0;
  while(res != EXIT && res != BACK) {
    res = file_mode_chosing(&maze_mode);
    if(res == OK && maze_mode == 1) {
      res = maze_output_from_file();
      if(res == INPUT_ERR)
        mvwprintw(stdscr, 0, 0, "No such file");
      else if(res == MALLOC_ERR)
        mvwprintw(stdscr, 0, 0, "Memory allocation failed");
    }
    else if(res == OK && maze_mode == 0) {
      res = maze_generation_mode_picked();
      if(res == INPUT_ERR)
        mvwprintw(stdscr, 0, 0, "No such file");
      else if(res == MALLOC_ERR)
        mvwprintw(stdscr, 0, 0, "Memory allocation failed");
    }
  }
  return res;
}

int process_cave_mode() {
  clear();
  int res = OK, opening_mode = 0; // generate or open existing cave file
  while(res != EXIT && res != BACK) {
    res = file_mode_chosing(&opening_mode);
    if(res == OK) {
      res = cave_open_file_mode_picked(opening_mode);
      if(res == INPUT_ERR)
        mvwprintw(stdscr, 0, 0, "No such file");
      else if(res == MALLOC_ERR)
        mvwprintw(stdscr, 0, 0, "Memory allocation failed");
    }
  }
  return res;
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  if(has_colors()) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
  }
  int mode = 0, res = OK;
  while(res != EXIT) {
    res = OK;
    res = mode_chosing(&mode);
    if(res == OK && mode == 0) {
      res = process_maze_mode();
    }
    else if(res == OK && mode == 1)
      res = process_cave_mode();
  }
  endwin();
}
