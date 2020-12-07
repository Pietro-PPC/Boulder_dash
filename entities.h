#ifndef __ENTITIES__
#define __ENTITIES__

#define KEY_SEEN 1
#define KEY_RELEASED 2

typedef struct player_s
{
    int x;
    int y;
    int timer;
} player_t;

void init_player(player_t *p);

#endif