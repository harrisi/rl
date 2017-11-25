#include "map.h"
#include "util.h"

void create_box(WIN *p_win, bool flag) {
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if (flag) {
        mvwaddch(
}
