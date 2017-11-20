#include <panel.h>
#include <string.h>

#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **, int);
void win_show(WINDOW *, char *, int);
void print_in_middle(WINDOW *, int, int, int, char *, chtype);

int main() {
    WINDOW *my_wins[3];
    PANEL *my_panels[3];
    bool panel_datas[3] = { false };
    bool *temp;
    int ch;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    NCURSES_COLOR_T cols[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN};
    for (int i = 0; i < sizeof(cols) / sizeof(NCURSES_COLOR_T); i++)
        init_pair(i + 1, cols[0], COLOR_BLACK);

    init_wins(my_wins, 3);

    for (int i = 0; i < 3; i++)
        my_panels[i] = new_panel(my_wins[i]);

    for (int i = 0; i < 3; i++)
        set_panel_userptr(my_panels[i], &panel_datas[i]);

    update_panels();

    attron(COLOR_PAIR(4));
    mvprintw(LINES - 3, 0, "Show or hide a window with 'a', 'b', or 'c'.");
    mvprintw(LINES - 2, 0, "F1 to exit");

    attroff(COLOR_PAIR(4));
    doupdate();

    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case 'a':
                temp = (bool *)panel_userptr(my_panels[0]);
                if (*temp) {
                    show_panel(my_panels[0]);
                } else {
                    hide_panel(my_panels[0]);
                }
                *temp = !*temp;
                break;
            case 'b':
                temp = (bool *)panel_userptr(my_panels[1]);
                if (*temp) {
                    show_panel(my_panels[1]);
                } else {
                    hide_panel(my_panels[1]);
                }
                *temp = !*temp;
                break;
            case 'c':
                temp = (bool *)panel_userptr(my_panels[2]);
                if (*temp) {
                    show_panel(my_panels[2]);
                } else {
                    hide_panel(my_panels[2]);
                }
                *temp = !*temp;
                break;
        }
        update_panels();
        doupdate();
    }

    endwin();

    return 0;
}

void init_wins(WINDOW **wins, int n) {
    int x, y, i;
    char label[80];

    y = 2;
    x = 10;
    for (i = 0; i < n; ++i) {
        wins[i] = newwin(NLINES, NCOLS, y, x);
        sprintf(label, "Window number %d", i + 1);
        win_show(wins[i], label, i + 1);
        y += 3;
        x += 7;
    }
}

void win_show(WINDOW *win, char *label, int label_color) {
    int startx, starty, height, width;

    getbegyx(win, starty, startx);
    getmaxyx(win, height, width);

    box(win, 0, 0);
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    mvwaddch(win, 2, width - 1, ACS_RTEE);

    print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
    int length, x, y;
    float temp;

    if (win == NULL) win = stdscr;
    getyx(win, y, x);
    if (startx != 0) x = startx;
    if (starty != 0) y = starty;
    if (width == 0) width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}
