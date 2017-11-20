#include <panel.h>
#include <stdlib.h>
#include <string.h>

// XXX: This resets position after moving/resizing a second time.

typedef struct _PANEL_DATA {
    int x, y, w, h;
    char label[80];
    int label_color;
    PANEL *next;
} PANEL_DATA;

#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **, int);
void win_show(WINDOW *, char *, int);
void print_in_middle(WINDOW *, int, int, int, char *, chtype);
void set_user_ptrs(PANEL **, int);

int main() {
    WINDOW *my_wins[3];
    PANEL *my_panels[3];
    PANEL_DATA *top;
    PANEL *stack_top;
    WINDOW *temp_win, *old_win;
    int ch;
    int newx, newy, neww, newh;
    int size = FALSE, move = FALSE;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    NCURSES_COLOR_T cols[4] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN};
    for (int i = 0; i < sizeof(cols) / sizeof(NCURSES_COLOR_T); i++)
        init_pair(i + 1, cols[i], COLOR_BLACK);

    init_wins(my_wins, 3);

    for (int i = 0; i < 3; i++)
        my_panels[i] = new_panel(my_wins[i]);

    set_user_ptrs(my_panels, 3);

    update_panels();

    attron(COLOR_PAIR(4));
    mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
    mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to exit)");
    attroff(COLOR_PAIR(4));
    doupdate();

    stack_top = my_panels[2];
    top = (PANEL_DATA *)panel_userptr(stack_top);
    // TODO: abstract this out to a function
    newx = top->x;
    newy = top->y;
    neww = top->w;
    newh = top->h;

    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case 9: // tab
                top = (PANEL_DATA *)panel_userptr(stack_top);
                top_panel(top->next);
                stack_top = top->next;
                top = (PANEL_DATA *)panel_userptr(stack_top);
                newx = top->x;
                newy = top->y;
                neww = top->w;
                newh = top->h;
                break;
            case KEY_BTAB:
                top = (PANEL_DATA *)panel_userptr(stack_top);
                top_panel(top->next);
                stack_top = top->next;
                top = (PANEL_DATA *)panel_userptr(stack_top);
                top_panel(top->next);
                stack_top = top->next;
                top = (PANEL_DATA *)panel_userptr(stack_top);
                newx = top->x;
                newy = top->y;
                neww = top->w;
                newh = top->h;
                break;
            case 'r':
                size = TRUE;
                attron(COLOR_PAIR(4));
                mvprintw(LINES - 4, 0,
                        "Entered resizing: arrow keys to resize, enter to end.");
                refresh();
                attroff(COLOR_PAIR(4));
                break;
            case 'm':
                attron(COLOR_PAIR(4));
                mvprintw(LINES - 4, 0,
                        "Entered moving: arrow keys to move, enter to end.");
                refresh();
                attroff(COLOR_PAIR(4));
                move = TRUE;
                break;
            case KEY_LEFT:
                --newx;
                ++neww;
                goto moveresize;
                break;
            case 'a':
                --newx;
                goto moveresize;
                break;
            case KEY_RIGHT:
                ++newx;
                --neww;
                goto moveresize;
                break;
            case 'd':
                ++newx;
                goto moveresize;
                break;
            case KEY_UP:
                --newy;
                ++newh;
                goto moveresize;
                break;
            case 'w':
                --newy;
                goto moveresize;
                break;
            case KEY_DOWN:
                ++newy;
                --newh;
                goto moveresize;
                break;
            case 's':
                ++newy;
                goto moveresize;
                break;
            case 10: // enter
moveresize:
                move(LINES - 4, 0);
                clrtoeol();
                refresh();
                old_win = panel_window(stack_top);
                temp_win = newwin(newh, neww, newy, newx);
                replace_panel(stack_top, temp_win);
                win_show(temp_win, top->label, top->label_color);
                delwin(old_win);
                move_panel(stack_top, newy, newx);
                break;
        }

        attron(COLOR_PAIR(4));
        mvprintw(LINES - 3, 0, "Use 'm' for moving, 'r' for resizing");
        mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to exit)");
        attroff(COLOR_PAIR(4));
        refresh();
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

void set_user_ptrs(PANEL **panels, int n) {
    PANEL_DATA *ptrs;
    WINDOW *win;
    int x, y, w, h, i;
    char temp[80];

    ptrs = (PANEL_DATA *)calloc(n, sizeof(PANEL_DATA));

    for (i = 0; i < n; ++i) {
        win = panel_window(panels[i]);
        getbegyx(win, y, x);
        getmaxyx(win, h, w);
        ptrs[i].x = x;
        ptrs[i].y = y;
        ptrs[i].w = w;
        ptrs[i].h = h;
        sprintf(temp, "Window number %d", i + 1);
        strcpy(ptrs[i].label, temp);
        ptrs[i].label_color = i + 1;
        if (i + 1 == n) {
            ptrs[i].next = panels[0];
        } else {
            ptrs[i].next = panels[i + 1];
        }
        set_panel_userptr(panels[i], &ptrs[i]);
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
