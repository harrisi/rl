#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int ch, prev, row, col;
    prev = EOF;
    FILE *fp;
    int y, x;
    bool single = false;

    if (argc != 2) {
        printf("Usage: %s <c file name>\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Cannot open input file");
        exit(1);
    }

    initscr();
    getmaxyx(stdscr, row, col);
    while ((ch = fgetc(fp)) != EOF) {
        getyx(stdscr, y, x); // get current cursor position
        if (y == (row - 1)) { // end of screen
            printw("<-Press Any Key->"); // tell user to press key
            getch();
            clear(); // clear screen
            move(0, 0); // move cursor to start of screen
        }
        if (prev == '/' && (ch == '*' || ch == '/')) { // if find "/ *"
            if (ch == '/') {
                single = true;
            }
            attron(A_BOLD); // turn bold on
            getyx(stdscr, y, x); // get cursor position
            move(y, x - 1); // back up one space
            printw("%c%c", '/', ch); // print bolded '/'
        } else {
            printw("%c", ch);
        }
        refresh();
        if (single && (ch == '\n')) {
            single = false;
            attroff(A_BOLD);
        }
        if ((prev == '*' && ch == '/') || (single && (ch == '\n'))) {
            attroff(A_BOLD); // turn off at end of comment
        }

        prev = ch;
    }

    /* multiline
     * comment
     * test
     */

    mvprintw(row - 1, 0, "<-Press Any Key->"); // end of file

    getch();

    endwin();
    fclose(fp);

    return 0;
}
