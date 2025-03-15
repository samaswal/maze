#include "tracking.h"
#include <ncurses.h>

int is_unobstructed(MazeInfo *m_info, int y1, int x1, int y2, int x2) {
    int res = 1;
    if(x2 > x1) {
        if(m_info->matrix1[y1][x1]) res = 0;
    }
    else if(x2 < x1) {
        if(m_info->matrix1[y1][x2]) res = 0;
    }
    else if(y2 > y1) {
        if(m_info->matrix2[y1][x1]) res = 0;
    }
    else if(y2 < y1) {
        if(m_info->matrix2[y2][x1]) res = 0;
    }
    else if(y1 == y2 && x1 == x2) res = 0;
    return res;
}

void track(MazeInfo *m_info, UserInfo *u_info) {
    int xb = (u_info->fPickedX - 1) / 2; //begin pos
    int yb = (u_info->fPickedY - 1) / 2;
    int xe = (u_info->sPickedX - 1) / 2; //end pos
    int ye = (u_info->sPickedY - 1) / 2;
    int d = 1;
    m_info->track_matrix[yb][xb] = d;
    while(m_info->track_matrix[ye][xe] == 0) {
        for(int i = 0; i < m_info->rows; i++) {
            for(int j = 0; j < m_info->columns; j++) {
                if(m_info->track_matrix[i][j] == d) {
                    if(i + 1 < m_info->rows && is_unobstructed(m_info, i, j, i + 1, j)) {
                        if(m_info->track_matrix[i + 1][j] == 0) m_info->track_matrix[i + 1][j] = d + 1;
                    }
                    if(i - 1 >= 0 && is_unobstructed(m_info, i, j, i - 1, j)) {
                        if(m_info->track_matrix[i - 1][j] == 0) m_info->track_matrix[i - 1][j] = d + 1;
                    }
                    if(j + 1 < m_info->columns && is_unobstructed(m_info, i, j, i, j + 1)) {
                        if(m_info->track_matrix[i][j + 1] == 0) m_info->track_matrix[i][j + 1] = d + 1;
                    }
                    if(j - 1 >= 0 && is_unobstructed(m_info, i, j, i, j - 1)) {
                        if(m_info->track_matrix[i][j - 1] == 0) m_info->track_matrix[i][j - 1] = d + 1;
                    }
                }
            }
        }
        d++;
    }
    int i = ye, j = xe;
    for(; i != yb || j != xb;) {
        if(i + 1 < m_info->rows && m_info->track_matrix[i + 1][j] == d - 1 && is_unobstructed(m_info, i, j, i + 1, j)) {
            m_info->track_matrix[i][j] = -d;
            i++;
            d--;
        }
        else if(i - 1 >= 0 && m_info->track_matrix[i - 1][j] == d - 1 && is_unobstructed(m_info, i, j, i - 1, j)) {
            m_info->track_matrix[i][j] = -d;
            i--;
            d--;
        }
        else if(j + 1 < m_info->columns && m_info->track_matrix[i][j + 1] == d - 1 && is_unobstructed(m_info, i, j, i, j + 1)) {
            m_info->track_matrix[i][j] = -d;
            j++;
            d--;
        }
        else if(j - 1 >= 0 && m_info->track_matrix[i][j - 1] == d - 1 && is_unobstructed(m_info, i, j, i, j - 1)) {
            m_info->track_matrix[i][j] = -d;
            j--;
            d--;
        }
    }
    m_info->track_matrix[i][j] = -1;
}

void clear_matr(int **matr, int rows, int cols) {
    if(matr) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                matr[i][j] = 0;
            }
        }
    }
}
