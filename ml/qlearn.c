#include "qlearn.h"
#include <ncurses.h>

void random_start_pos(MLInfo *ml_info) {
    ml_info->agent.x = rand() % (ml_info->m_info->columns);
    ml_info->agent.y = rand() % (ml_info->m_info->rows);
    while(ml_info->agent.y == ml_info->end_pos.y && ml_info->agent.x == ml_info->end_pos.x) {
        ml_info->agent.x = rand() % (ml_info->m_info->columns);
        ml_info->agent.y = rand() % (ml_info->m_info->rows);
    }
}

Action random_action() {
    Action res = Up;
    int n = rand() % 4;
    if(n == 0) res = Up;
    else if(n == 1) res = Down;
    else if(n == 2) res = Left;
    else if(n == 3) res = Right;
    return res;
}

double max_action_weight(Actions acts) {
    double maxQ = -10000000;
    if(acts.up > maxQ) maxQ = acts.up;
    if(acts.down > maxQ) maxQ = acts.down;
    if(acts.left > maxQ) maxQ = acts.left;
    if(acts.right > maxQ) maxQ = acts.right;
    return maxQ;
}

double *act_weight_at(Actions *acts, Action act) {
    double *res;
    switch(act) {
        case Up:
            res = &acts->up;
            break;
        case Down:
            res = &acts->down;
            break;
        case Left:
            res = &acts->left;
            break;
        case Right:
            res = &acts->right;
            break;
    }
    return res;
}

Action choose_action(State agent, Actions **Q, double epsilon) {
    Action res = Up;
    if(Q) {
        double chance_rand = (double)(rand() % 1000) / 1000;
        if(chance_rand <= epsilon) {
            res = random_action();
        }
        else {
            double maxQ = -10000000000;
            if(Q[agent.y][agent.x].up > maxQ) {
                maxQ = Q[agent.y][agent.x].up;
                res = Up;
            }
            if(Q[agent.y][agent.x].down > maxQ) {
                maxQ = Q[agent.y][agent.x].down;
                res = Down;
            }
            if(Q[agent.y][agent.x].left > maxQ) {
                maxQ = Q[agent.y][agent.x].left;
                res = Left;
            }
            if(Q[agent.y][agent.x].right > maxQ) {
                maxQ = Q[agent.y][agent.x].right;
                res = Right;
            }
        }

    }
    return res;
}

State take_action(MLInfo *ml_info, Action action, double *reward) {
    State res = {0, 0};
    if(ml_info && ml_info->Q) {
        res.x = ml_info->agent.x;
        res.y = ml_info->agent.y;
        switch (action) {
            case Up:
                res.y--;
                break;
            case Down:
                res.y++;
                break;
            case Left:
                res.x--;
                break;
            case Right:
                res.x++;
                break;
        }
        if(res.y < ml_info->m_info->rows && res.x < ml_info->m_info->columns && res.y >= 0 && res.x >= 0) {
            if(!is_unobstructed(ml_info->m_info, ml_info->agent.y, ml_info->agent.x, res.y, res.x)) {
                *reward = -40;
                res.x = ml_info->agent.x;
                res.y = ml_info->agent.y;
            }
            else *reward = -1;
        }
        else if(res.y >= ml_info->m_info->rows || res.x >= ml_info->m_info->columns || res.y < 0 || res.x < 0){
            *reward = -40;
            res.x = ml_info->agent.x;
            res.y = ml_info->agent.y;
        }

        if(res.x == ml_info->end_pos.x && res.y == ml_info->end_pos.y) *reward = 50;
    }
    return res;
}


void q_learning(MLInfo *ml_info, int episodes, double alpha/*speed*/, double gamma/*discount multiplier*/, double epsilon/*chance of random choice*/) {
    if(ml_info && ml_info->Q && ml_info->m_info->matrix1) {
        int c = 0;
        for(int ep = 0; ep < episodes && c != 'q'; ep++) {
            random_start_pos(ml_info);
            while(c != 'q' && (ml_info->agent.x != ml_info->end_pos.x || ml_info->agent.y != ml_info->end_pos.y)) {
                draw_maze(ml_info->m_info);
                draw_agent(ml_info);
                refresh();
                double reward = 0;
                Action action = choose_action(ml_info->agent, ml_info->Q, epsilon);
                State next_state = take_action(ml_info, action, &reward);
                double max_next_q = max_action_weight(ml_info->Q[next_state.y][next_state.x]);
                double *curr_q = act_weight_at(&ml_info->Q[ml_info->agent.y][ml_info->agent.x], action);
                double curr_val = *curr_q;
                *act_weight_at(&ml_info->Q[ml_info->agent.y][ml_info->agent.x], action) += alpha * (reward + gamma * max_next_q - curr_val);
                ml_info->agent.x = next_state.x;
                ml_info->agent.y = next_state.y;
                //printf("%d %d\n", next_state.y, next_state.x);
                mvprintw(0, 18, "episode: %d | reward: %lf", ep, *act_weight_at(&ml_info->Q[ml_info->agent.y][ml_info->agent.x], action));
                timeout(10);
                c = getch();
            }
        }
    }
}
