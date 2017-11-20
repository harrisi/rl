#include <ncurses.h>
#include <assert.h>

int main() {
    int ch, count = 0;
    mmask_t old;

    initscr();
    noecho();
    cbreak();
    mousemask(ALL_MOUSE_EVENTS, &old);
    keypad(stdscr, TRUE);

    while ((ch = getch()) != 'q') {
        count++;
        if (ch == KEY_MOUSE) {
            MEVENT event;
            assert(getmouse(&event) == OK);
            mvprintw(0, 0, "Mouse event!\n");
        }
        mvprintw(1, 1, "Event number %4d", count);
    }

    return 0;
}
