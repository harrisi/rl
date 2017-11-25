#ifndef _UTIL_H_
#define _UTIL_H_

typedef struct _win_border_struct {
    chtype ls, // left-side
           rs, // right-side
           ts, // top-side
           bs, // bottom-side
           tl, // top-left
           tr, // top-right
           bl, // bottom-left
           br; // bottom-right
} WIN_BORDER;

typedef struct _WIN_struct {
    int startx, starty;
    int height, width;
    WIN_BORDER border;
} WIN;

#endif
