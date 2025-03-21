#include "cave/cave_backend.h"
#include "drawing.h"
#include "maze_backend.h"
#include "structs.h"
#include "tracking.h"
#include "ml/save_load.h"
#include "ml/qlearn.h"
#include "ml/controls.h"
#include <ncurses.h>

void print_error(int code) {
  switch (code) {
    case MALLOC_ERR:
      mvwprintw(stdscr, 0, 0, "Memory allocation failed");
      break;
    case INPUT_ERR:
      mvwprintw(stdscr, 0, 0, "No such file");
      break;
    case NAMING_ERR:
      mvwprintw(stdscr, 0, 0, "Incorrect naming. Forbidden symbols: ,./\"\'\\");
      break;
  }
}

int init_maze_from_file(MazeInfo *m_info, char *fileName) {
  int res = OK;
  res = is_correct_name(fileName);
  if(res == OK) {
    char path[50] = "mazes/";
    strcat(path, fileName);
    strcat(path, ".maze");
    FILE *f;
    int r, c;
    f = fopen(path, "r");
    if(!f) res = INPUT_ERR;
    if(res == OK) {
      if(fscanf(f, "%d", &r) != 1 || r < 0 || r > 50) res = INPUT_ERR;
      if(fscanf(f, "%d", &c) != 1 || c < 0 || c > 50) res = INPUT_ERR;
    }
    if(res == OK) {
      res = init_maze_struct(m_info, r, c);
    }
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
          int sc = fscanf(f, "%d", &m_info->matrix1[i][j]);
          if(sc != 1) {
            res = INPUT_ERR;
            break;
          }
        }
        if(res == INPUT_ERR) break;
      }
    }
    if(res == OK) {
      for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
          int sc = fscanf(f, "%d", &m_info->matrix2[i][j]);
          if(sc != 1) {
            res = INPUT_ERR;
            break;
          }
        }
        if(res == INPUT_ERR) break;
      }
    }
    if(f) fclose(f);
  }
  return res;
}

int write_maze_file(MazeInfo *m_info, char *fileName) {
  int res = OK;
  char path[50] = "mazes/";
  res = create_dir_if_not_exists("mazes");
  if(res == OK) res = is_correct_name(fileName);
  if(res == OK) {
    strcat(path, fileName);
    strcat(path, ".maze");
    FILE *f = fopen(path, "w");
    if(!f) res = INPUT_ERR;
    else {
      fprintf(f, "%d %d\n", m_info->rows, m_info->columns);
      for(int i = 0; i < m_info->rows; i++) {
        for(int j = 0; j < m_info->columns; j++) {
          fprintf(f, "%d ", m_info->matrix1[i][j]);
        }
        fprintf(f, "\n");
      }
      fprintf(f, "\n");
      for(int i = 0; i < m_info->rows; i++) {
        for(int j = 0; j < m_info->columns; j++) {
          fprintf(f, "%d ", m_info->matrix2[i][j]);
        }
        fprintf(f, "\n");
      }
    }
    if(f) fclose(f);
  }
  return res;
}

int generate_maze_file(char *fileName, int rows, int columns, MazeInfo *m_info) {
  int res = init_maze_struct(m_info, rows, columns);
  if(res == OK) {
    generate_eller(m_info);
    res = write_maze_file(m_info, fileName);
  }
  return res;
}

int handle_maze_input(int c, UserInfo *u_info, MazeInfo *m_info) {
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
      if(pick_point(u_info)) track(m_info, u_info);
      else clear_matr(m_info->track_matrix, m_info->rows, m_info->columns);
      break;
    default:
      break;
  }
  return res;
}

int handle_cave_input(int c, CaveInfo *c_info) {
  int res = OK;
  switch (c) {
    case 'q':
      res = EXIT;
      break;
    case 'b':
      res = BACK;
      break;
    case 'Q':
      res = EXIT;
      break;
    case 'B':
      res = BACK;
      break;
    case 10:
      if(c_info->gen_mode == STEP_BY_STEP) {
        generate_cave(c_info);
      }
  }
  return res;
}

int mode_chosing(int *mode) {
  int res = OK, key = 0;
  display_modes_menu(0);
  refresh();
  while(res == OK) {
    display_modes_menu(*mode);

    key = getch();
    if (key == KEY_UP && *mode > 0) {
      (*mode)--;
    } else if (key == KEY_DOWN && *mode < 3) {
      (*mode)++;
    } else if (key == 10) {
      if(*mode == 3) res = EXIT;
      break;
    }
  }
  return res;
}

int four_variants_choosing(int *mode, const char *prompt, const char *var1, const char *var2) {
  int res = OK, key = 0;
  display_files_modes_menu(0, prompt, var1, var2);
  refresh();
  while(res == OK) {
    display_files_modes_menu(*mode, prompt, var1, var2);
    key = getch();
    if (key == KEY_UP && *mode > 0) {
      (*mode)--;
    } else if (key == KEY_DOWN && *mode < 4) {
      (*mode)++;
    } else if (key == 10) {
      if(*mode == 3) res = EXIT;
      else if(*mode == 2) res = BACK;
      break;
    }
  }
  return res;
}

void cave_gen_mode_choosing(int *genmode) {
  int res = OK, key = 0;
  display_cave_gen_mode_menu(0);
  refresh();
  while(res == OK) {
    display_cave_gen_mode_menu(*genmode);
    key = getch();
    if (key == KEY_UP && *genmode > 0) {
      *genmode = 0;
    } else if (key == KEY_DOWN && *genmode < 2) {
      *genmode = 1;
    } else if (key == 10) {
      res = EXIT;
    }
  }
}

int maze_output_from_file() {
  int res = OK;
  char filename[40];
  MazeInfo maze_info;
  UserInfo user_info;
  while (res == OK) {
    display_filename_input_menu("Filename:", filename);
    res = init_maze_from_file(&maze_info, filename);
    if(res == OK) init_user_info(&user_info, &maze_info);
    clear();
    while (res == OK) {
      draw_maze(&maze_info);
      draw_cursor(user_info.y, user_info.x);
      draw_track(&maze_info);
      draw_picked_points(&user_info);
      res = handle_maze_input(getch(), &user_info, &maze_info);
      refresh();
    }
    clear();
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_struct(&maze_info);
  }
  return res;
}

int cave_display_state(CaveInfo *c_info) {
  int res = OK;
  cave_set_timer(c_info);
  while(res == OK) {
    draw_cave(c_info);
    timeout(10);
    res = handle_cave_input(getch(), c_info);
    generate_automatic(c_info);
    refresh();
  }
  clear();
  return res;
}

int cave_initialize_mode_picked(int mode) {
  int res = OK;
  CaveInfo cave_info;
  char filename[40];
  int genmode = 0;
  if(mode == 1) { //open file mode
    display_filename_input_menu("Filename:", filename);
    res = init_cave_from_file(&cave_info, filename);
    clear();
  }
  else if(mode == 0) { //generate file mode
    int row = 0, col = 0;
    display_size_input_menu("Rows:", 2, &row, 4, 50);
    display_size_input_menu("Columns:", 2, &col, 4, 50);
    display_filename_input_menu("Filename:", filename);
    res = generate_cave_file(filename, row, col, &cave_info);
    clear();
  }
  if(res == OK) {
    display_size_input_menu("Birth limit:", 2, &cave_info.birth_limit, 0, 7);
    display_size_input_menu("Death limit:", 2, &cave_info.death_limit, 0, 7);
    clear();
    cave_gen_mode_choosing(&genmode);
    cave_info.gen_mode = (genmode == 0) ? AUTOMATIC : STEP_BY_STEP;
    if(genmode == 0) {
      clear();
      int tm = 0;
      display_size_input_menu("Gen Time Delay:", 2, &tm, 10, 1000);
      cave_info.time_delay = tm;
    }
    clear();
    res = cave_display_state(&cave_info);
  }
  clear();
  if(res != INPUT_ERR && res != MALLOC_ERR) destroy_cave_struct(&cave_info);
  return res;
}

int maze_generation_mode_picked() {
  int res = OK;
  MazeInfo maze_info;
  UserInfo user_info;
  while(res == OK) {
    int row = 0, col = 0;
    char filename[40];
    display_size_input_menu("Rows:", 2, &row, 4, 50);
    display_size_input_menu("Columns:", 3, &col, 4, 50);
    display_filename_input_menu("Filename:", filename);
    res = generate_maze_file(filename, row, col, &maze_info);
    if(res == OK) init_user_info(&user_info, &maze_info);
    clear();
    while (res == OK) {
      draw_maze(&maze_info);
      draw_cursor(user_info.y, user_info.x);
      draw_track(&maze_info);
      draw_picked_points(&user_info);
      res = handle_maze_input(getch(), &user_info, &maze_info);
      refresh();
    }
    clear();
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_struct(&maze_info);
  }
  return res;
}

int maze_ml_test_mode_picked() {
  int res = OK;
  char filename[40];
  MLInfo ml_info;
  MazeInfo m_info;
  UserInfo u_info;
  ml_info.m_info = &m_info;
  while (res == OK) {
    display_filename_input_menu("Filename:", filename);
    res = init_ml_maze_from_file(&ml_info, filename);
    if(res == OK) init_user_info(&u_info, ml_info.m_info);
    clear();
    while (res == OK) {
      timeout(200);
      draw_maze(ml_info.m_info);
      draw_cursor(u_info.y, u_info.x);
      draw_picked_points(&u_info);
      draw_track(ml_info.m_info);
      draw_agent(&ml_info);
      res = handle_maze_ml_input(getch(), &u_info, &ml_info);
      test_q_table(&ml_info);
      refresh();
    }
    clear();
    if(res != INPUT_ERR && res != MALLOC_ERR) destroy_maze_ml_struct(&ml_info);
  }
  return res;
}

int process_maze_mode() {
  clear();
  int res = OK, maze_mode = 0;
  while(res != EXIT && res != BACK) {
    res = four_variants_choosing(&maze_mode, "Source:", "Generate file", "Choose file");
    if(res == OK && maze_mode == 1) {
      res = maze_output_from_file();
      if(res != OK) print_error(res);
    }
    else if(res == OK && maze_mode == 0) {
      res = maze_generation_mode_picked();
      if(res != OK) print_error(res);
    }
  }
  return res;
}

int process_cave_mode() {
  clear();
  int res = OK, opening_mode = 0; // generate or open existing cave file
  while(res != EXIT && res != BACK) {
    res = four_variants_choosing(&opening_mode, "Source:", "Generate file", "Choose file");
    if(res == OK) {
      res = cave_initialize_mode_picked(opening_mode);
      if(res != OK) print_error(res);
    }
  }
  return res;
}

int process_maze_ml_mode() {
  clear();
  int res = OK, ml_mode = 0; // test or generate and learn new
  while(res != EXIT && res != BACK) {
    res = four_variants_choosing(&ml_mode, "Mode:", "Generate & learn", "Test");
    if(res == OK && ml_mode == 1) {
      res = maze_ml_test_mode_picked();
      if(res != OK) print_error(res);
    }
  }
  return res;
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  if(has_colors()) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
  }
  int mode = 0, res = OK;
  while(res != EXIT) {
    res = OK;
    res = mode_chosing(&mode);
    if(res == OK && mode == 0) {
      res = process_maze_mode();
    }
    else if(res == OK && mode == 1)
      res = process_cave_mode();
    else if(res == OK && mode == 2) {
      res = process_maze_ml_mode();
    }
  }
  endwin();
}
