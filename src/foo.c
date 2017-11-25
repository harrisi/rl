#include <stdlib.h>
#include <curses.h>
#include <signal.h>

static void finish(int sig);

int main(int argc, char *argv[]) {
    int num = 0;
    (void) signal(SIGINT, finish);

    (void) initscr();
    keypad(stdscr, TRUE);
    (void) nonl();
    (void) cbreak();
    (void) echo();

    if (has_colors()) {
        start_color();

        NCURSES_COLOR_T cols[] = {
            COLOR_RED,
            COLOR_GREEN,
            COLOR_YELLOW,
            COLOR_BLUE,
            COLOR_CYAN,
            COLOR_MAGENTA,
            COLOR_WHITE,
        };
        for (int i = 1; i < 8; i++) {
            init_pair(i, cols[i], COLOR_BLACK);
        }
    }

    for (;;) {
        int c = getch();
        attrset(COLOR_PAIR(num % 8));
        num++;
    }

    finish(0);
}

static void finish(int sig) {
    endwin();

    exit(0);
}
