#include "cave_backend.h"
#include <stdlib.h>
#include <sys/time.h>

int init_cave_struct(CaveInfo *cave_info, int r, int c) {
  int res = OK;
  res = (r > 0 && r <= 50 && c > 0 && c <= 50) ? OK : INPUT_ERR;
  if(cave_info && res == OK) {
    cave_info->rows = r;
    cave_info->columns = c;
    cave_info->birth_limit = 0;
    cave_info->death_limit = 0;
    cave_info->time_delay = 0;
    cave_info->time = 0;
    cave_info->matrix = (int **)calloc(r, sizeof(int *));
    if(!cave_info->matrix) res = MALLOC_ERR;
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        cave_info->matrix[i] = (int *)calloc(c, sizeof(int));
          if(!cave_info->matrix[i]) {
            for(int j = 0; j < i; j++)
              free(cave_info->matrix[j]);
            res = MALLOC_ERR;
            break;
          }
      }
    }
  }
  return res;
}

void cave_set_timer(CaveInfo *c_info) {
  struct timeval cur_t;
  gettimeofday(&cur_t, NULL);
  c_info->time = cur_t.tv_sec * 1000 + cur_t.tv_usec / 1000;
}

void destroy_cave_struct(CaveInfo *c_info) {
  if(c_info && c_info->matrix) {
    for(int i = 0; i < c_info->rows; i++) {
      free(c_info->matrix[i]);
    }
    free(c_info->matrix);
  }
  c_info = NULL;
}

int is_correct_cave(const CaveInfo *c_info) {
    int res = 1;
    if(!(c_info && c_info->matrix && c_info->columns > 0 && c_info->rows > 0)) res = 0;
    return res;
}

void set_cave_primal(CaveInfo *c_info) {
  if(is_correct_cave(c_info)) {
    srand(time(NULL));
    for(int i = 0; i < c_info->rows; i++) {
      for(int j = 0; j < c_info->columns; j++) {
        c_info->matrix[i][j] = (rand() % 100 <= 40);
      }
    }
  }
}

int generate_cave_file(char *filename, int rows, int columns, CaveInfo *c_info) {
    int res = init_cave_struct(c_info, rows, columns);
    if(res == OK) {
        set_cave_primal(c_info);
        res = write_cave_file(c_info, filename);

    }
    return res;
}

int write_cave_file(const CaveInfo *c_info, char *fileName) {
    int res = OK;
    char path[50] = "caves/";
    res = create_dir_if_not_exists("caves");
    if(res == OK) res = is_correct_name(fileName);
      if(res == OK) {
        strcat(path, fileName);
        strcat(path, ".cave");
        FILE *f = fopen(path, "w");
        if(!f) res = INPUT_ERR;
        else {
            fprintf(f, "%d %d\n", c_info->rows, c_info->columns);
            for(int i = 0; i < c_info->rows; i++) {
                for(int j = 0; j < c_info->columns; j++) {
                    fprintf(f, "%d ", c_info->matrix[i][j]);
                }
                fprintf(f, "\n");
            }
        }
        if(f) fclose(f);
    }
    return res;
}

int init_cave_from_file(CaveInfo *c_info, char *fileName) {
    int res = OK;
    res = is_correct_name(fileName);
    if(res == OK) {
      char path[50] = "caves/";
      strcat(path, fileName);
      strcat(path, ".cave");
      FILE *f;
      int r, c;
      f = fopen(path, "r");
      if(!f) res = INPUT_ERR;
      if(res == OK) {
          if(fscanf(f, "%d", &r) != 1 || r < 0 || r > 50) res = INPUT_ERR;
          if(fscanf(f, "%d", &c) != 1 || c < 0 || c > 50) res = INPUT_ERR;
      }
      if(res == OK) {
          res = init_cave_struct(c_info, r, c);
      }
      if(res == OK) {
          for(int i = 0; i < r; i++) {
              for(int j = 0; j < c; j++) {
                  int sc = fscanf(f, "%d", &c_info->matrix[i][j]);
                  if(sc != 1) {
                      res = INPUT_ERR;
                      break;
                  }
              }
              if(res == INPUT_ERR) break;
          }
      }
      if(f) fclose(f);
  }
  return res;
}
