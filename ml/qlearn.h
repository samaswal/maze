#ifndef Q_LEARN_H_
#define Q_LEARN_H_

#include "../maze_backend.h"

typedef enum Action {
  Up = 1,
  Down = 2,
  Left = 3,
  Right = 4
} Action;

typedef struct State {
  int y;
  int x;
} State;

typedef struct LearnData {
  int **S;
  Action **A;
}

typedef struct MLInfo {
  int rows;
  int columns;
  int **matrix1;
  int **matrix2;
  double **Q[4];
  State end_pos;
  State start_pos;
} MLInfo;

#endif
