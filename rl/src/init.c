#include "init.h"

void init_screen(screen_t *scr) {
    initscr();
    getmaxyx(stdscr, scr->maxy, scr->maxx);
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}
