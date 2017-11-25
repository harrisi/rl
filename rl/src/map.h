#ifndef _MAP_H_
#define _MAP_H_

#include <ncurses.h>
#include "util.h"

typedef struct _map_t {
    // blah
    chtype **cells; // holds individual cell
} map_t;

void create_box(WIN *, bool);

#endif
