#ifndef Q_LEARN_H_
#define Q_LEARN_H_

#include <stdlib.h>
#include "../structs.h"
#include "../tracking.h"
#include "../drawing.h"
#include <getopt.h>


void random_start_pos(MLInfo *ml_info);
Action choose_action(State agent, Actions **Q, double epsilon);
void q_learning(MLInfo *ml_info, int episodes, double alpha, double gamma, double epsilon);
void test_q_table(MLInfo *ml_info);
State take_action(MLInfo *ml_info, Action action, double *reward);

#endif
