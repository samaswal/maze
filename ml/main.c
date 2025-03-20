#include "qlearn.h"
#include "save_load.h"
#include <ncurses.h>
#include <getopt.h>
#include <unistd.h>

void print_help() {
  printf("Usage:\n"
  "-f <filename> - open file\n"
  "-g <filename> - generate maze file\n"
  "-h - help");
}

typedef struct Flags {
  int loadfile;
  int generate;
  int train;
  int learn;
} Flags;

int get_user_input(int argc, char *argv[], MLInfo *ml_info, Flags *flags, char *filename) {
  int res = OK;
  int current_flag = getopt(argc, argv, "hg:f:");
  for(; current_flag != -1 && res == OK; current_flag = getopt(argc, argv, "hg:f:")) {
    switch (current_flag) {
      case 'f':
        if(!flags->loadfile && !flags->generate && res == OK) {
          flags->loadfile = 1;
          res = init_ml_maze_from_file(ml_info, optarg);
        }
        break;
      case 'h':
        res = HELP;
        print_help();
        break;
      case 'g':
        int row = 0, col = 0, endY = 0, endX = 0;
        if(res == OK && !flags->loadfile && !flags->generate) {
          flags->generate = 1;
          printf("Enter new maze rows number:\n");
          char nm[10];
          fgets(nm, 10, stdin);
          row = atoi(nm);
          while(row < 4 || row > 50) {
            printf("Rows number should be from 4 to 50!\n");
            fgets(nm, 10, stdin);
            row = atoi(nm);
          }
          printf("Enter new maze columns number:\n");
          fgets(nm, 10, stdin);
          col = atoi(nm);
          while(col < 4 || col > 50) {
            printf("Columns number should be from 4 to 50!\n");
            fgets(nm, 10, stdin);
            col = atoi(nm);
          }

          printf("Enter new maze endpoint y pos:\n");
          fgets(nm, 10, stdin);
          endY = atoi(nm);
          while(endY < 0 || endY > row - 1) {
            printf("Y pos value should be from %d to %d!\n", 0, row - 1);
            fgets(nm, 10
            , stdin);
            endY = atoi(nm);
          }

          printf("Enter new maze endpoint x pos:\n");
          fgets(nm, 10, stdin);
          endX = atoi(nm);
          while(endX < 0 || endX > col - 1) {
            printf("X pos value should be from %d to %d!\n", 0, col - 1);
            fgets(nm, 10
            , stdin);
            endX = atoi(nm);
          }

          res = generate_maze_ml_file(optarg, row, col, endY, endX, ml_info);
          strcpy(filename, optarg);
        }
        break;
      default:
        res = INPUT_ERR;
        print_help();
        break;
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int res = OK;
  MLInfo ml_info;
  MazeInfo m_info;
  Flags flags = {0, 0, 0, 0};
  ml_info.m_info = &m_info;
  char filename[50];
  res = get_user_input(argc, argv, &ml_info, &flags, filename);
  if(res == OK) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    // if(has_colors()) {
    //   start_color();
    //   init_pair(1, COLOR_RED, COLOR_BLACK);
    //   clear();
    // }
      // while (res == OK) {
      //   timeout(10);
      //   int c = getch();
      //   if(c == 'q' || c == 'Q') break;
      //   draw_maze(ml_info.m_info);
      //   refresh();
      // }
    q_learning(&ml_info, 15000, 0.4, 0.9, 0.2);
    if(res != INPUT_ERR && res != MALLOC_ERR) save_q_table(&ml_info, filename);
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_ml_struct(&ml_info);
    endwin();
  }
  return 0;
}
