#include <ncurses.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[]) {
    WINDOW *my_win;
    int startx, starty, width, height;
    int ch;

    initscr(); // start curses
    cbreak(); // line buffering disabled, everything passed to me

    keypad(stdscr, TRUE); // F1, F2, arrow keys, etc.

    height = 1;
    width = 1;
    starty = (LINES - height) / 2; // center of window
    startx = (COLS - width) / 2; // center of window
    printw("Press F1 to exit");
    refresh();
    my_win = create_newwin(height, width, starty, startx);

    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_LEFT:
                destroy_win(my_win);
                my_win = create_newwin(height, width, starty, --startx);
                break;
            case KEY_RIGHT:
                destroy_win(my_win);
                my_win = create_newwin(height, width, starty, ++startx);
                break;
            case KEY_UP:
                destroy_win(my_win);
                my_win = create_newwin(height, width, --starty, startx);
                break;
            case KEY_DOWN:
                destroy_win(my_win);
                my_win = create_newwin(height, width, ++starty, startx);
                break;
        }
    }

    endwin();
    
    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    //box(local_win, 1, 1); // 0, 0 gives default characters for vert/horiz lines
    mvwaddch(local_win, 0, 0, '@');
    wrefresh(local_win); // show that box

    return local_win;
}

void destroy_win(WINDOW *local_win) {
    // box(local_win, ' ', ' '); : this won't produce the desired result of
    // erasing the window. It will leave it's four corners, an ugly remnant of a
    // window.
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    // The parameters are:
    // 1. win: window on which to operate
    // 2. ls: character to be used for left side of window
    // 3. rs: character to be used for right side of window
    // 4. ts: character to be used for top side of window
    // 5. bs: character to be used for bottom side of window
    // 6. tl: character to be used for top left of window
    // 7. tr: character to be used for top right of window
    // 8. bl: character to be used for bottom left of window
    // 9. br: character to be used for bottom right of window

    wrefresh(local_win);
    delwin(local_win);
}
