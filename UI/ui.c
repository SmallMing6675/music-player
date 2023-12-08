#include <ncurses.h>
#include <string.h>

#define MAX_ROWS 25
#define MAX_COLS 80

void init() {
    initscr();      // Initialize ncurses
    raw();          // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys
    noecho();       // Do not display typed characters
}

void draw(const char* buffer, int cursor_x, int cursor_y) {
    clear();      
    mvprintw(0, 0, buffer);

    // Display the cursor
    move(cursor_y, cursor_x);
    refresh();     
}

int get_keys(){
	int ch = getch();
	return ch;
};

