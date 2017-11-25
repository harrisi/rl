#include <ncurses.h> // includes stdio.h
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "init.h"

int main() {
    screen_t scr;
    WINDOW *self;

    init_screen(&scr);
    srand(time(NULL));

    char **map = malloc(scr.maxy * sizeof(*map));
    for (int i = 0; i < scr.maxy; i++) {
        map[i] = malloc(scr.maxx * sizeof(**map));
    }

    unsigned short entrancex;
    unsigned short entrancey;

    int curx, cury;

    do {
        for (int i = 0; i < scr.maxy; i++) {
            memset(map[i], '#', scr.maxx);
        }

        entrancex = rand() % scr.maxx;
        entrancey = rand() % scr.maxy;

        map[entrancey][entrancex] = '@';
        curx = entrancex;
        cury = entrancey;

        for (int i = 0; i < scr.maxx * scr.maxy / 2; i++) {
            switch (rand() % 4) {
                case 0: // up
                    cury--;
                    if (cury < 0) cury = scr.maxy - 1;
                    break;
                case 1: // down
                    cury++;
                    if (cury > (scr.maxy - 1)) cury = 0;
                    break;
                case 2: // left
                    curx--;
                    if (curx < 0) curx = scr.maxx - 1;
                    break;
                case 3: // right
                    curx++;
                    if (curx > (scr.maxx - 1)) curx = 0;
                    break;
                case 4:
                    curx = rand() % scr.maxx;
                    cury = rand() % scr.maxy;
                    break;
            }
            if (!((curx == entrancex) && (cury == entrancey)))
                map[cury][curx] = '.';
        }

        for (int i = 0; i < scr.maxy; i++) {
            for (int j = 0; j < scr.maxx; j++) {
                if (map[i][j] == '.') {
                    if (!(rand() % (scr.maxx * scr.maxy / 10))) {
                        map[i][j] = '*';
                    }
                }
            }
        }

        for (int i = 0; i < scr.maxy; i++) {
            mvprintw(i, 0, map[i]);
        }

        self = newwin(1, 1, entrancey, entrancex);
        //box(self, 1, 1);

        mvchgat(entrancey, entrancex, 1, A_REVERSE, 0, NULL);
        wrefresh(self);

        mvchgat(entrancey, entrancex, 1, A_REVERSE, 0, NULL);

        move(entrancey, entrancex);
        refresh();
        wrefresh(self);

        int playerx, playery;
        // full map is generated now. move around!
        int c;
        while ((c = getch()) != 'q') {
            getyx(stdscr, playery, playerx);
            switch (c) {

                // this is a real mess.
                case KEY_UP:
                    if (playery - 1 < 0) {
                        if (map[scr.maxy - 1][playerx] != '#') {
                            playery = scr.maxy - 1;
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
                    if (playerx + 1 > scr.maxx - 1) {
                        if (map[playery][0] != '#') {
                            playerx = 0;
                            map[playery][playerx] = '@';
                            mvaddch(playery, scr.maxx - 1, '.');
                            map[playery][scr.maxx - 1] = '.';
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
                    if (playery + 1 > scr.maxy - 1) {
                        if (map[0][playerx] != '#') {
                            playery = 0;
                            map[playery][playerx] = '@';
                            mvaddch(scr.maxy - 1, playerx, '.');
                            map[scr.maxy - 1][playerx] = '.';
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
                        if (map[playery][scr.maxx - 1] != '#') {
                            playerx = scr.maxx - 1;
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
            wrefresh(self);
        }
    } while (getch() != 'q');

    for (int i = 0; i < scr.maxy; i++) {
        free(map[i]);
    }
    free(map);

    wrefresh(self);
    delwin(self);

    refresh();
    endwin();

    return 0;
}
