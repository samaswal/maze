#include "cave_backend.h"
#include <stdlib.h>

int init_cave_struct(CaveInfo *cave_info, int r, int c) {
  int res = OK;
  res = (r > 0 && r <= 50 && c > 0 && c <= 50) ? OK : INPUT_ERR;
  if(cave_info && res == OK) {
    cave_info->rows = r;
    cave_info->columns = c;
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

void destroy_cave_struct(CaveInfo *c_info) {
  if(c_info && c_info->matrix) {
    for(int i = 0; i < c_info->rows; i++) {
      free(c_info->matrix[i]);
    }
    free(c_info->matrix);
  }
  c_info = NULL;
}

int is_correct_cave(CaveInfo *c_info) {
    int res = 1;
    if(!(c_info && c_info->matrix && c_info->columns > 0 && c_info->rows > 0)) res = 0;
    return res;
}

void generate_new_cave(CaveInfo *c_info) {
  if(is_correct_cave(c_info)) {
    srand(time(NULL));
    for(int i = 0; i < c_info->rows; i++) {
      for(int j = 0; j < c_info->columns; j++) {
        c_info->matrix[i][j] = rand() % 2;
      }
    }
  }
}

int generate_cave_file(char *filename, int rows, int columns, CaveInfo *c_info) {
    int res = init_cave_struct(c_info, rows, columns);
    if(res == OK) {
        generate_new_cave(c_info);
        res = write_cave_file(c_info, filename);

    }
    return res;
}

int write_cave_file(CaveInfo *c_info, char *fileName) {
    int res = OK;
    FILE *f = fopen(fileName, "w");
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
    return res;
}
