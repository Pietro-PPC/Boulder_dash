#include "game.h"

#ifndef __UPDATE__
#define __UPDATE__

#define FRAMERATE 1.0/60.0

void update_game(game_t *game, unsigned char *key);

void end_game(game_t *game);

#endif