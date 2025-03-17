#include "cave_generation.h"

void generate_cave(CaveInfo *c_info) {
  if(is_correct_cave(c_info)) {
    int r = c_info->rows;
    int c = c_info->columns;
    for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
        int cnt_alive = 0;
          for(int k = -1; k <= 1; k++) {
            for(int m = -1; m <= 1; m++) {
              if(((i + k >= 0) && (j + m >= 0) && (i + k < r) && (j + m < c) &&
                  (c_info->matrix[i + k][j + m])) && !(k == 0 && m == 0))
                cnt_alive++;
              else {
                if(i + k < 0 || i + k >= r || j + m < 0 || j + m >= c) cnt_alive++;
              }
            }
          }
        if(c_info->matrix[i][j] && cnt_alive <= c_info->death_limit) c_info->matrix[i][j] = 0;
        else if(!c_info->matrix[i][j] && cnt_alive >= c_info->birth_limit) c_info->matrix[i][j] = 1;
      }
    }
  }
}

void generate_automatic(CaveInfo *c_info) {
  if(is_correct_cave(c_info) && c_info->gen_mode == AUTOMATIC) {
    clock_t t = clock();
    if((t - c_info->time) * 1000 / CLOCKS_PER_SEC > c_info->time_delay) {
      generate_cave(c_info);
      c_info->time = t;
    }
  }
}

