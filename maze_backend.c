#include "maze_backend.h"

int init_maze_struct(MazeInfo *maze_info, int r, int c) {
  int res = OK;
  res = (r > 0 && r <= 50 && c > 0 && c <= 50) ? OK : INPUT_ERR;
  if(maze_info && res == OK) {
    maze_info->rows = r;
    maze_info->columns = c;
    maze_info->matrix1 = (int **)calloc(r, sizeof(int *));
    maze_info->matrix2 = (int **)calloc(r, sizeof(int *));
    maze_info->track_matrix = (int **)calloc(r, sizeof(int *));
    if(!maze_info->matrix1) res = MALLOC_ERR;
    if(!maze_info->matrix2) res = MALLOC_ERR;
    if(!maze_info->track_matrix) res = MALLOC_ERR;
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        maze_info->matrix1[i] = (int *)calloc(c, sizeof(int));
        maze_info->matrix2[i] = (int *)calloc(c, sizeof(int));
        maze_info->track_matrix[i] = (int *)calloc(c, sizeof(int));
        if(!maze_info->matrix1[i] || !maze_info->matrix2[i] || !maze_info->track_matrix) {
          for(int j = 0; j < i; j++) {
            free(maze_info->matrix1[j]);
            free(maze_info->matrix2[j]);
            free(maze_info->track_matrix[j]);
          }
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
      free(m_info->track_matrix[i]);
    }
    free(m_info->matrix1);
    free(m_info->matrix2);
    free(m_info->track_matrix);
  }
  m_info = NULL;
}

int is_correct_maze(MazeInfo *m_info) {
  int res = 1;
  if(!(m_info && m_info->matrix1 && m_info->matrix2 && m_info->track_matrix && m_info->columns > 0 && m_info->rows > 0)) res = 0;
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

void clean_arr(int *arr, int size) {
  for(int *ptr = arr; ptr - arr < size; *(ptr++) = 0);
}

void unite_set(int *set, int id, int columns) {
  int old_set_id = set[id + 1];
  int new_set_id = set[id];
  for(int i = 0; i < columns; i++) {
    if(set[i] == old_set_id) set[i] = new_set_id;
  }
}

int is_horizontable(int index, int *line, int *set, int columns) {
  int res = 0;
  int abs_number = 0;
  int hor_number = 0;
  for(int i = 0; i < columns; i++) {
    if(set[i] == index) {
      abs_number++;
      if(line[i] == 1) hor_number++;
    }
  }
  res = (abs_number - hor_number > 1) ? 1 : 0;
  //if(res) res = abs_number > 1 ? 1 : 0;
  return res;
}

void generate_eller(MazeInfo *m_info) {
  if(is_correct_maze(m_info)) {
    srand(time(NULL));
    int set[m_info->columns];
    //    bool hlines[m_info->columns];
    int set_index = 1;
    for(int j = 0; j < m_info->columns; j++) {
      set[j] = set_index++;
    }
    for(int i = 0; i < m_info->rows; i++) {
      for(int j = 0; j < m_info->columns - 1; j++) {
        int n = rand() % 2;
        if(n || (set[j] == set[j + 1])) m_info->matrix1[i][j] = 1;
        else {
          unite_set(set, j, m_info->columns);
        }
      }
      
      for(int j = 0; j < m_info->columns; j++) {
        int n = rand() % 2;
        if(n) {
          m_info->matrix2[i][j] = is_horizontable(set[j], m_info->matrix2[i], set, m_info->columns);
        }
      }
      
      if(i + 1 < m_info->rows) {
        for(int j = 0; j < m_info->columns; j++) {
          if(m_info->matrix2[i][j]) set[j] = ++set_index;
        }
      }
      else {
        for(int j = 0; j < m_info->columns; j++) {
          m_info->matrix2[i][j] = 1;
          if(j + 1 < m_info->columns && set[j] != set[j + 1]) {
            m_info->matrix1[i][j] = 0;
            unite_set(set, j, m_info->columns);
          }
        }
      }
    }
    for(int i = 0; i < m_info->rows; i++) {
      m_info->matrix1[i][m_info->columns - 1] = 1;
    }
  }
}
