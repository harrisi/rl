#ifndef _INIT_H_
#define _INIT_H_

#include <ncurses.h>
#include "map.h"

typedef struct _screen_t {
    int maxy, maxx; // screen size
    // this needs more data, maybe map?
    map_t map;
} screen_t;

void init_screen(screen_t *);

#endif
