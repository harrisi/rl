#include <ncurses.h> // includes stdio.h
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int row, col; // store number of rows and cols of screen
    int maxy, maxx;

    initscr(); // start curses
    getmaxyx(stdscr, maxy, maxx); // get number of rows and cols
    getmaxyx(stdscr, row, col);
    srand(time(NULL));
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    char **map = malloc(maxy * sizeof(*map));
    for (int i = 0; i < maxy; i++) {
        map[i] = malloc(maxx * sizeof(**map));
    }

    unsigned short entrancex;
    unsigned short entrancey;

    int curx, cury;

    do {
        for (int i = 0; i < row; i++) {
            memset(map[i], '#', col);
        }

        entrancex = rand() % maxx;
        entrancey = rand() % maxy;

        map[entrancey][entrancex] = '@';
        curx = entrancex;
        cury = entrancey;

        for (int i = 0; i < maxx * maxy / 2; i++) {
            switch (rand() % 4) {
                case 0: // up
                    cury--;
                    if (cury < 0) cury = maxy - 1;
                    break;
                case 1: // down
                    cury++;
                    if (cury > (maxy - 1)) cury = 0;
                    break;
                case 2: // left
                    curx--;
                    if (curx < 0) curx = maxx - 1;
                    break;
                case 3: // right
                    curx++;
                    if (curx > (maxx - 1)) curx = 0;
                    break;
                case 4:
                    curx = rand() % maxx;
                    cury = rand() % maxy;
                    break;
            }
            if (!((curx == entrancex) && (cury == entrancey)))
                map[cury][curx] = '.';
        }

        for (int i = 0; i < maxy; i++) {
            for (int j = 0; j < maxx; j++) {
                if (map[i][j] == '.') {
                    if (!(rand() % (maxx * maxy / 10))) {
                        map[i][j] = '*';
                    }
                }
            }
        }

        for (int i = 0; i < maxy; i++) {
            mvprintw(i, 0, map[i]);
        }

        mvchgat(entrancey, entrancex, 1, A_REVERSE, 0, NULL);

        move(entrancey, entrancex);
        refresh();

        int playerx, playery;
        // full map is generated now. move around!
        int c;
        while ((c = getch()) != 'q') {
            getyx(stdscr, playery, playerx);
            switch (c) {

                // this is a real mess.
                case KEY_UP:
                    if (playery - 1 < 0) {
                        if (map[maxy - 1][playerx] != '#') {
                            playery = maxy - 1;
                            map[playery][playerx] = '@';
                            mvaddch(0, playerx, '.');
                            map[0][playerx] = '.';
                            mvaddch(playery, playerx, '@' | A_REVERSE);
                        } else {
                            continue;
                        }
                    } else {
                        if (map[playery - 1][playerx] == '#') continue;
                        map[playery][playerx] = '.';
                        mvaddch(playery, playerx, '.');
                        map[--playery][playerx] = '@';
                        mvaddch(playery, playerx,
                                map[playery][playerx] | A_REVERSE);
                    }
                    move(playery, playerx);
                    break;

                case KEY_RIGHT:
                    if (playerx + 1 > maxx - 1) {
                        if (map[playery][0] != '#') {
                            playerx = 0;
                            map[playery][playerx] = '@';
                            mvaddch(playery, maxx - 1, '.');
                            map[playery][maxx - 1] = '.';
                            mvaddch(playery, playerx, '@' | A_REVERSE);
                        } else {
                            continue;
                        }
                    } else {
                        if (map[playery][playerx + 1] == '#') continue;
                        map[playery][playerx] = '.';
                        mvaddch(playery, playerx, '.');
                        map[playery][++playerx] = '@';
                        mvaddch(playery, playerx,
                                map[playery][playerx] | A_REVERSE);
                    }
                    move(playery, playerx);
                    break;

                case KEY_DOWN:
                    if (playery + 1 > maxy - 1) {
                        if (map[0][playerx] != '#') {
                            playery = 0;
                            map[playery][playerx] = '@';
                            mvaddch(maxy - 1, playerx, '.');
                            map[maxy - 1][playerx] = '.';
                            mvaddch(playery, playerx, '@' | A_REVERSE);
                        } else {
                            continue;
                        }
                    } else {
                        if (map[playery + 1][playerx] == '#') continue;
                        map[playery][playerx] = '.';
                        mvaddch(playery, playerx, '.');
                        map[++playery][playerx] = '@';
                        mvaddch(playery, playerx,
                                map[playery][playerx] | A_REVERSE);
                    }
                    move(playery, playerx);
                    break;

                case KEY_LEFT:
                    if (playerx - 1 < 0) {
                        if (map[playery][maxx - 1] != '#') {
                            playerx = maxx - 1;
                            map[playery][playerx] = '@';
                            mvaddch(playery, 0, '.');
                            map[playery][0] = '.';
                            mvaddch(playery, playerx, '@' | A_REVERSE);
                        } else {
                            continue;
                        }
                    } else {
                        if (map[playery][playerx - 1] == '#') continue;
                        map[playery][playerx] = '.';
                        mvaddch(playery, playerx, '.');
                        map[playery][--playerx] = '@';
                        mvaddch(playery, playerx,
                                map[playery][playerx] | A_REVERSE);
                    }
                    move(playery, playerx);
                    break;
                default:
                    //printw("??????????????");
                    break;
            }

            refresh();
        }
    } while (getch() != 'q');

    for (int i = 0; i < row; i++) {
        free(map[i]);
    }
    free(map);

    refresh();
    endwin();

    return 0;
}
