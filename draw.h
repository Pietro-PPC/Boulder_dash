#include <allegro5/allegro_font.h>
#include "map.h"
#include "sprites.h"
#include "game.h"

#ifndef __DRAW__
#define __DRAW__

void pre_draw(ALLEGRO_BITMAP *bitmap);

void post_draw(ALLEGRO_BITMAP *bitmap, ALLEGRO_DISPLAY *disp);

void draw_game(sprites_t *sprites, game_t *game, ALLEGRO_FONT *font);

#endif