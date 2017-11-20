#include <ncurses.h>

int main() {
    WINDOW *stdscr = initscr(); // start curses mode
    printw("Hello World!!!"); // print hello world
    refresh(); // print it on to the real screen
    getch(); // wait for user input
    endwin(); // end curses mode

    return 0;
}
