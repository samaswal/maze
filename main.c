#include <ncurses.h>
#include <stdio.h>
#define BORDER_TOP 5
#define BORDER_LEFT 5

void draw_maze_primal(int r, int c) {
	if(r > 0 && c > 0) {
        mvhline(BORDER_TOP, BORDER_LEFT, 0, r * 3);
        mvvline(BORDER_TOP + 1, BORDER_LEFT + 1, 0, 1);
        //mvhline(BORDER_TOP + c, BORDER_LEFT, 0, r);
        mvvline(BORDER_TOP, BORDER_LEFT, 0, c * 3);
        //mvvline(BORDER_TOP, BORDER_LEFT + r, 0, c);
        mvaddch(BORDER_TOP, BORDER_LEFT, ACS_ULCORNER);
        mvaddch(BORDER_TOP + r * 3, BORDER_LEFT, ACS_LLCORNER);
        mvaddch(BORDER_TOP, BORDER_LEFT + c * 3, ACS_URCORNER);
        mvaddch(BORDER_LEFT + r * 3, BORDER_LEFT + c * 3, ACS_LRCORNER);
	}
}

void draw_cell_locked(int y, int x) {
    if(y > 0 && x > 0) {
        
    }
}

int get_user_input(int argc, char *argv[], int *rows, int *columns) {
	int res = 1;
	if(argc == 3) {
    		if(sscanf(argv[1], "%d", rows) != 1) {
			res = 0;
		}
		if(sscanf(argv[2], "%d", columns) != 1) {
			res = 0;
		}
  	}
	else res = 0;
	return res;
}

int main(int argc, char *argv[]) {
	initscr();
    noecho();
    curs_set(0);
 	int rows = 0;
	int columns = 0;
	int flag = 1;
	flag = get_user_input(argc, argv, &rows, &columns);

	while(flag) {
		draw_maze_primal(rows, columns);
		if(getch() == 'p') flag = 0;
		timeout(500);
	}
	
	endwin();
}
