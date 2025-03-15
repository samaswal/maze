#ifndef STRUCTS_H_
#define STRUCTS_H_

#define BORDER_TOP 5
#define BORDER_LEFT 5
#define MALLOC_ERR -1
#define INPUT_ERR 1
#define HELP_MODE 2

typedef struct MazeInfo {
    int rows;
    int columns;
    int **matrix1;
    int **matrix2;
    int **track_matrix;
} MazeInfo;

typedef struct UserInfo {
    int y;
    int x;
    int maxY;
    int maxX;
    int fPickedX;
    int fPickedY;
    int sPickedX;
    int sPickedY;
} UserInfo;

int is_correct_maze(MazeInfo *m_info);

void clear_matr(int **matr, int rows, int cols);

#endif
