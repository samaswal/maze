#include "qlearn.h"
#include "controls.h"
#include <ncurses.h>
#include <getopt.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int res = OK;
  MLInfo ml_info;
  MazeInfo m_info;
  UserInfo u_info;
  Flags flags = {0, 0, 0, 0};
  ml_info.m_info = &m_info;
  char filename[50];
  res = get_flags_input(argc, argv, &ml_info, &flags, filename);
  if(res == OK) {
    init_user_info(&u_info, ml_info.m_info);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
     if(has_colors()) {
       start_color();
       init_pair(1, COLOR_RED, COLOR_BLACK);
     }
      // while (res == OK) {
      //   timeout(10);
      //   int c = getch();
      //   if(c == 'q' || c == 'Q') break;
      //   draw_maze(ml_info.m_info);
      //   refresh();
      // }
    if(flags.generate) {
      q_learning(&ml_info, 40000, 0.4, 0.9, 0.2);
      if(res != INPUT_ERR && res != MALLOC_ERR) save_q_table(&ml_info, filename);
    }
    else if(flags.loadfile) {
        while (res == OK) {
          timeout(200);
          draw_maze(ml_info.m_info);
          draw_agent(&ml_info);
          draw_cursor(u_info.y, u_info.x);
          //draw_track(&maze_info);
          draw_picked_points(&u_info);
          res = handle_maze_ml_input(getch(), &u_info, &ml_info);
          test_q_table(&ml_info);
          refresh();
        }
      }
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_ml_struct(&ml_info);
    
    endwin();
    if(res == INPUT_ERR) printf("print err");
    if(res == MALLOC_ERR) printf("alloc err");
    else printf("ERR");
  }
  else printf("ERR");
  return 0;
}
