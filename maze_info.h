#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#define BORDER_TOP 5
#define BORDER_LEFT 5
#define MALLOC_ERR -1
#define INPUT_ERR 1

typedef struct MazeInfo {
  int rows;
  int columns;
  int **matrix1;
  int **matrix2;
} MazeInfo;

MazeInfo *get_maze_struct() {
  static MazeInfo maze_info;
  return &maze_info;
}

int init_maze_struct(MazeInfo *maze_info, int r, int c) {
  int res = OK;
  if(maze_info) {
    maze_info->rows = r;
    maze_info->columns = c;
    maze_info->matrix1 = (int **)calloc(r, sizeof(int *));
    maze_info->matrix2 = (int **)calloc(r, sizeof(int *));
    if(!maze_info->matrix1) res = MALLOC_ERR;
    if(!maze_info->matrix2) res = MALLOC_ERR;
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        maze_info->matrix1[i] = (int *)calloc(c, sizeof(int));
        if(!maze_info->matrix1[i]) {
          for(int j = 0; j < i; j++)
            free(maze_info->matrix1[j]);
          res = MALLOC_ERR;
          break;
        }
      }
    }
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        maze_info->matrix2[i] = (int *)calloc(c, sizeof(int));
        if(!maze_info->matrix2[i]) {
          for(int j = 0; j < i; j++)
            free(maze_info->matrix2[j]);
          res = MALLOC_ERR;
          break;
        }
      }
    }
  }
  if(res != OK)  {
    maze_info->rows = 0;
    maze_info->columns = 0;
  }
  return res;
}

void destroy_maze_struct(MazeInfo *m_info) {
  if(m_info && m_info->matrix1) {
    for(int i = 0; i < m_info->rows; i++) {
      free(m_info->matrix1[i]);
      free(m_info->matrix2[i]);
    }
    free(m_info->matrix1);
    free(m_info->matrix2);
  }
  m_info = NULL;
}

int is_correct_maze(MazeInfo *m_info) {
  int res = 1;
  if(!(m_info && m_info->matrix1 && m_info->matrix2 && m_info->columns > 0 && m_info->rows > 0)) res = 0;
  return res;
}

void print_matrices(MazeInfo *m_info) {
  if(is_correct_maze(m_info)) {
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns; j++) {
        printf("%d ", m_info->matrix1[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns; j++) {
        printf("%d ", m_info->matrix2[i][j]);
      }
      printf("\n");
    }
  }
}
