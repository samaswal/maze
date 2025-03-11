#include "drawing.h"
#include <unistd.h>
#include <string.h>
#include <time.h>

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

void generate_new_maze(MazeInfo *m_info) {
  if(m_info && m_info->matrix1 && m_info->matrix2) {
    srand(time(NULL));
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns; j++) {
        int n = rand() % 2;
        m_info->matrix1[i][j] = n;
        n = rand() % 2;
        m_info->matrix2[i][j] = n;
        if(i + 1 == m_info->rows) {
          m_info->matrix2[i][j] = 1;
        }
        if(j + 1 == m_info->columns) {
          m_info->matrix1[i][j] = 1;
        }
      }
    }
  }
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
    generate_new_maze(m_info);
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
          scanf("%d", &row);
          while(row < 4 || row > 50) {
            printf("Rows number should be 4 to 50!\n");
            scanf("%d", &row);
          }
          printf("Enter new maze columns number:\n");
          scanf("%d", &col);
          while(col < 4 || col > 50) {
            printf("Columns number should be 4 to 50!\n");
            scanf("%d", &col);
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

int main(int argc, char *argv[]) {
  MazeInfo *maze_info = get_maze_struct();
  int flag = 1;
  int is_ok = OK;
  //flag = get_user_input(argc, argv, maze_info);
  is_ok = get_user_input(argc, argv, maze_info);
  if(is_ok == INPUT_ERR) destroy_maze_struct(maze_info);
  if(is_ok == OK) flag = 1;
  else flag = 0;
  //print_matrices(maze_info);
  if(is_ok == OK) {
    initscr();
    noecho();
    curs_set(0);
    
    while(flag) {
      draw_maze(maze_info);
      if(getch() == 27 /*ESC*/) flag = 0;
      timeout(500);
    }
  } else print_error(is_ok);

  destroy_maze_struct(maze_info);	
  endwin();
}
