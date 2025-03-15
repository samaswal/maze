#include "drawing.h"
#include "maze_backend.h"
#include "structs.h"
#include "tracking.h"

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
  return res;
}

int generate_file(char *fileName, int rows, int columns, MazeInfo *m_info) {
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
        res = HELP_MODE;
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
          res = generate_file(optarg, row, col, m_info);
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

int handle_input(int c, UserInfo *u_info, MazeInfo *m_info) {
  int res = 1;
  switch (c) {
    case 'q':
      res = 0;
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

int main(int argc, char *argv[]) {
  MazeInfo *maze_info = get_maze_struct();
  UserInfo *user_info = get_user_struct();
  int flag = 1;
  int is_ok = OK;
  is_ok = get_user_input(argc, argv, maze_info);
  if(is_ok == INPUT_ERR) destroy_maze_struct(maze_info);
  if(is_ok == OK) flag = 1;
  else flag = 0;
  //print_matrices(maze_info);
  if(is_ok == OK) {
    init_user_info(user_info, maze_info);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    while(flag) {
      draw_maze(maze_info);
      draw_cursor(user_info->y, user_info->x);
      draw_track(maze_info);
      draw_picked_points(user_info);
      flag = handle_input(getch(), user_info, maze_info);
      refresh();
    }
  } else print_error(is_ok);

  destroy_maze_struct(maze_info);	
  endwin();
}
