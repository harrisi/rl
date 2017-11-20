#include <ncurses.h>

int main(int argc, char *argv[]) {
    initscr();
    start_color(); // start color functionality

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    printw("A big string which I didn't care to type fully ");
    getch();
    mvchgat(0, 0, -1, A_BLINK, 1, NULL);
    /* First two parameters specify the position at which to start
     * Third parameter number of characters to update. -1 meants until EOL
     * Fourth parameter is the normal attribute you wanted to give to the
     * character
     * Fifth is the color index. It is the index given during init_pair()
     * use 0 if you didn't want color
     * Sixth one is always NULL (XXX: Why?)
     */

    refresh();
    getch();
    endwin();

    return 0;
}
