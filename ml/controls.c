#include "controls.h"

void print_help() {
  printf("Usage:\n"
  "-f <filename> - open file\n"
  "-g <filename> - generate maze file\n"
  "-h - help");
}

int get_flags_input(int argc, char *argv[], MLInfo *ml_info, Flags *flags, char *filename) {
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

void set_start_pos(MLInfo *ml_info, int y, int x) {
  ml_info->agent.y = y;
  ml_info->agent.x = x;
}

int handle_maze_ml_input(int c, UserInfo *u_info, MLInfo *ml_info) {
  int res = OK;
  switch (c) {
    case 'q':
      res = EXIT;
      break;
    case 'Q':
      res = EXIT;
      break;
    case 'b':
      res = BACK;
      break;
    case 'B':
      res = BACK;
      break;
    case KEY_LEFT:
      mv_curs_left(u_info);
      break;
    case KEY_RIGHT:
      mv_curs_right(u_info);
      break;
    case KEY_DOWN:
      mv_curs_down(u_info);
      break;
    case KEY_UP:
      mv_curs_up(u_info);
      break;
    case 10:
//      if(pick_point(u_info)) track(m_info, u_info);
//      else clear_matr(m_info->track_matrix, m_info->rows, m_info->columns);
      if(ml_info) {
        pick_agent_start_point(u_info);
        set_start_pos(ml_info, u_info->fPickedY / 2, u_info->fPickedX / 2);
      }
      break;
    default:
      break;
  }
  return res;
}
