#include <ncurses.h>

int main() {
    int ch;

    initscr(); // start curses mode
    raw(); // line buffering disabled
    keypad(stdscr, TRUE); // get F1, F2, arrow keys, etc.
    noecho(); // don't echo() while do getch

    printw("Type any character to see it in bold\n");
    // if raw() hadn't been called we have to press enter before it get to prog
    ch = getch();

    if (ch == KEY_F(1)) // without keypad enabled this will not get to us either
        printw("F1 Key pressed");
    // without noecho() some ugly escape characters might be printed on screen
    else {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }

    refresh(); // print to screen
    getch(); // wait for use input
    endwin(); // end curses

    return 0;
}
