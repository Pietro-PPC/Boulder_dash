#include "map.h"

#ifndef __GAME__
#define __GAME__

typedef struct samples_s
{
    int diamond;
    int explosion;
    int boulder_hit;
    int step;
    int victory;
    int open_door;
} samples_t;

typedef struct game_s
{
    map_t map;
    samples_t n_plays;
    int lives;
    int score;
    int time;
    int frame;
    int diamonds_got;
} game_t;

void initialize_samples(samples_t *s);

void initialize_game(game_t *game);

#endif