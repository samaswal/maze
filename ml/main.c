#include "qlearn.h"

int get_user_input(int argc, char *argv[], MazeInfo *m_info) {
  int res = OK;
  int current_flag = getopt(argc, argv, "g:f:");
  for(; current_flag != -1 && res == OK; current_flag = getopt(argc, argv, "g:f:")) {
    switch (current_flag) {
      case 'f':
        if(res == OK)
          res = init_maze_from_file(m_info, optarg);
        break;
      case 'h':
        res = HELP;
        print_help();
        break;
      case 'g':
        int row = 0, col = 0;
        if(res == OK) {
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
          res = generate_maze_file(optarg, row, col, m_info);
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
  
  return 0;
}
