#ifndef _CHARACTER_H_
#define _CHARACTER_H_

typedef struct _character_t {
    int cury, curx;
    // window?
    char health;
    // other stats; this will change to stats_t probably so it's shared
} character_t;

#endif
