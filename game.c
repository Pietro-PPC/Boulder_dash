#include <stdio.h>
#include "game.h"
#include "map.h"

void initialize_samples(samples_t *s)
{
    s->boulder_hit = 0;
    s->diamond = 0;
    s->explosion = 0;
    s->step = 0;
    s->victory = 0;
    s->open_door = 0;
    s->wow = 0;
}

void initialize_game(game_t *game)
{
    initialize_map(&(game->map));
    initialize_samples((&game->n_plays));
    game->lives = 1;
    game->score = 0;
    game->diamonds_got = 0;
    game->time = 180;
    game->frame = 0;
}