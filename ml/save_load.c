#include "save_load.h"
#include "qlearn.h"
#include <stdio.h>

int init_ml_maze_from_file(MLInfo *ml_info, char *fileName) {
    int res = OK;
    res = is_correct_name(fileName);
    if(res == OK) {
        char path[50] = "../build/qmazes/";
        strcat(path, fileName);
        strcat(path, ".qmaze");
        FILE *f;
        int r, c, y, x;
        f = fopen(path, "r");
        if(!f) res = INPUT_ERR;
        if(res == OK) {
            if(fscanf(f, "%d", &r) != 1 || r < 0 || r > 50) res = INPUT_ERR;
            if(fscanf(f, "%d", &c) != 1 || c < 0 || c > 50) res = INPUT_ERR;
            if(fscanf(f, "%d", &y) != 1 || y < 0 || y > r - 1) res = INPUT_ERR;
            if(fscanf(f, "%d", &x) != 1 || x < 0 || x > c - 1) res = INPUT_ERR;
        }
        if(res == OK) {
            res = create_maze_ml_struct(ml_info, r, c, y, x);
        }
        if(res == OK) {
            for(int i = 0; i < r; i++) {
                for(int j = 0; j < c; j++) {
                    int sc = fscanf(f, "%d", &ml_info->m_info->matrix1[i][j]);
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
                    int sc = fscanf(f, "%d", &ml_info->m_info->matrix2[i][j]);
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
                    int sc = fscanf(f, "%lf", &ml_info->Q[i][j].up);
                    if(sc != 1) {
                        res = INPUT_ERR;
                        break;
                    }
                    sc = fscanf(f, "%lf", &ml_info->Q[i][j].down);
                    if(sc != 1) {
                        res = INPUT_ERR;
                        break;
                    }
                    sc = fscanf(f, "%lf", &ml_info->Q[i][j].left);
                    if(sc != 1) {
                        res = INPUT_ERR;
                        break;
                    }
                    sc = fscanf(f, "%lf", &ml_info->Q[i][j].right);
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

int create_maze_ml_struct(MLInfo *ml_info, int r, int c, int y, int x) {
    int res = init_maze_struct(ml_info->m_info, r, c);
    if(res == OK) res = (r > 0 && r <= 50 && c > 0 && c <= 50 && y >= 0 && y < r && x >= 0 && x < c) ? OK : INPUT_ERR;
    if(ml_info && res == OK) {
        ml_info->end_pos.x = x;
        ml_info->end_pos.y = y;
        ml_info->agent.x = 0;
        ml_info->agent.y = 0;
        ml_info->Q = (Actions **)calloc(r, sizeof(Actions *));
        if(!ml_info->Q) res = MALLOC_ERR;
        if(res == OK) {
            for(int i = 0; i < r; i++) {
                ml_info->Q[i] = (Actions *)calloc(c, sizeof(Actions));
                if(!ml_info->Q[i]) {
                    for(int j = 0; j < i; j++) {
                        free(ml_info->Q[j]);
                    }
                    res = MALLOC_ERR;
                    break;
                }
            }
        }
    }
    if(res != OK)  {
        ml_info->end_pos.y = 0;
        ml_info->end_pos.x = 0;
    }
    return res;
}

int write_maze_ml_file(MLInfo *ml_info, char *fileName) {
    int res = OK;
    char path[50] = "../build/qmazes/";
    res = create_dir_if_not_exists("../build/qmazes");
    if(res == OK) res = is_correct_name(fileName);
    if(res == OK) {
        strcat(path, fileName);
        strcat(path, ".qmaze");
        FILE *f = fopen(path, "w");
        if(!f) res = INPUT_ERR;
        else {
            fprintf(f, "%d %d\n", ml_info->m_info->rows, ml_info->m_info->columns);
            fprintf(f, "%d %d\n", ml_info->end_pos.y, ml_info->end_pos.x);
            for(int i = 0; i < ml_info->m_info->rows; i++) {
                for(int j = 0; j < ml_info->m_info->columns; j++) {
                    fprintf(f, "%d ", ml_info->m_info->matrix1[i][j]);
                }
                fprintf(f, "\n");
            }
            fprintf(f, "\n");
            for(int i = 0; i < ml_info->m_info->rows; i++) {
                for(int j = 0; j < ml_info->m_info->columns; j++) {
                    fprintf(f, "%d ", ml_info->m_info->matrix2[i][j]);
                }
                fprintf(f, "\n");
            }

            // fprintf(f, "\n");
            // for(int i = 0; i < ml_info->m_info->rows; i++) {
            //     for(int j = 0; j < ml_info->m_info->columns; j++) {
            //         fprintf(f, "%lf %lf %lf %lf ", ml_info->Q[i][j].up, ml_info->Q[i][j].down, ml_info->Q[i][j].left, ml_info->Q[i][j].right);
            //     }
            //     fprintf(f, "\n");
            // }

        }
        if(f) fclose(f);
    }
    return res;
}

void save_q_table(MLInfo *ml_info, char *filename) {
    int res = OK;
    char path[50] = "../build/qmazes/";
    res = create_dir_if_not_exists("../build/qmazes");
    if(res == OK) res = is_correct_name(filename);
    if(res == OK) {
        strcat(path, filename);
        strcat(path, ".qmaze");
        FILE *f = fopen(path, "a");
        if(!f) res = INPUT_ERR;
        else {
            fprintf(f, "\n");
            for(int i = 0; i < ml_info->m_info->rows; i++) {
                for(int j = 0; j < ml_info->m_info->columns; j++) {
                    fprintf(f, "%lf %lf %lf %lf ", ml_info->Q[i][j].up, ml_info->Q[i][j].down, ml_info->Q[i][j].left, ml_info->Q[i][j].right);
                }
                fprintf(f, "\n");
            }
        }
    if(f) fclose(f);
    }

}

int generate_maze_ml_file(char *fileName, int rows, int columns, int y, int x, MLInfo *ml_info) {
    int res = create_maze_ml_struct(ml_info, rows, columns, y, x);
    if(res == OK) {
        generate_eller(ml_info->m_info);
        q_random_init(ml_info);
        res = write_maze_ml_file(ml_info, fileName);
    }
    return res;
}

void q_random_init(MLInfo *ml_info) {
  if(ml_info && ml_info->Q) {
    for(int i = 0; i < ml_info->m_info->rows; i++) {
        for(int j = 0; j < ml_info->m_info->columns; j++) {
            ml_info->Q[i][j].up = ((rand() % 2001) - 1000) / 1000.0;
            ml_info->Q[i][j].down = ((rand() % 2001) - 1000) / 1000.0;
            ml_info->Q[i][j].left = ((rand() % 2001) - 1000) / 1000.0;
            ml_info->Q[i][j].right = ((rand() % 2001) - 1000) / 1000.0;
        }
    }
  }
}

void destroy_maze_ml_struct(MLInfo *ml_info) {
    for(int i = 0; i < ml_info->m_info->rows; i++) {
        free(ml_info->Q[i]);
    }
    free(ml_info->Q);
    destroy_maze_struct(ml_info->m_info);

}
