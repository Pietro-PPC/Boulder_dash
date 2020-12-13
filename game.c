#include <stdio.h>
#include "game.h"
#include "map.h"

void initialize_game(game_t *game)
{
    initialize_map(&(game->map));
    game->lives = 1;
    game->score = 0;
    game->diamonds_got = 0;
    game->time = 180;
    game->frame = 0;
}