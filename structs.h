#ifndef STRUCTS_H_
#define STRUCTS_H_

#define BORDER_TOP 5
#define BORDER_LEFT 5
#define MALLOC_ERR 1
#define INPUT_ERR 2
#define HELP 3
#define EXIT 4
#define BACK 5
#define MKDIR_ERR 6
#define NAMING_ERR 7

#include <ncurses.h>
#include <time.h>
#include <string.h>
//#include <cstddef>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <sys/stat.h>


typedef enum CaveGenMode {
    AUTOMATIC,
    STEP_BY_STEP
} CaveGenMode;

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
    int birth_limit;
    int death_limit;
    CaveGenMode gen_mode;
    int time_delay;
    long long time;
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

typedef enum Action {
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3
} Action;

typedef struct State {
    int y;
    int x;
} State;

typedef struct Actions {
    double up;
    double down;
    double left;
    double right;
} Actions;

typedef struct MLInfo {
    MazeInfo *m_info;
    Actions **Q;
    State end_pos;
    State agent;
} MLInfo;


//typedef struct CaveUserInfo {}

int is_correct_maze(const MazeInfo *m_info);
int is_correct_cave(const CaveInfo *c_info);

int create_dir_if_not_exists(const char *path);
int is_correct_name(const char *filename);
void clear_matr(int **matr, int rows, int cols);

#endif
