#ifndef STRUCTS_H_
#define STRUCTS_H_

#define BORDER_TOP 5
#define BORDER_LEFT 5
#define MALLOC_ERR 1
#define INPUT_ERR 2
#define HELP 3
#define EXIT 4
#define BACK 5

#include <ncurses.h>

typedef struct MazeInfo {
    int rows;
    int columns;
    int **matrix1;
    int **matrix2;
    int **track_matrix;
} MazeInfo;

typedef struct CaveInfo {
    int rows;
    int columns;
    int **matrix;
} CaveInfo;

typedef struct MazeUserInfo {
    int y;
    int x;
    int maxY;
    int maxX;
    int fPickedX;
    int fPickedY;
    int sPickedX;
    int sPickedY;
} UserInfo;

//typedef struct CaveUserInfo {}

int is_correct_maze(MazeInfo *m_info);

void clear_matr(int **matr, int rows, int cols);

#endif
