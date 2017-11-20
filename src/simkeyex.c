#include <stdio.h>
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

char *choices[] = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int main() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear(); // necessary?
    noecho();
    cbreak();
    startx = (COLS - WIDTH) / 2;
    starty = (LINES - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, press enter to select a choice.");
    refresh();
    print_menu(menu_win, highlight);

    while (true) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1) {
                    highlight = n_choices;
                } else {
                    --highlight;
                }
                break;
            case KEY_DOWN:
                if (highlight == n_choices) {
                    highlight = 1;
                } else {
                    ++highlight;
                }
                break;
            case 10: // enter
                choice = highlight;
                break;
            default:
                mvprintw(LINES - 1, 0, "Character pressed is %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0) // user made choice; break from infinite loop
            break;
    }

    mvprintw(LINES - 1, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    clrtoeol();

    refresh();
    getch();

    endwin();

    return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
    int x, y, i;

    x = y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) { // highlight present choice
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    
    wrefresh(menu_win);
}