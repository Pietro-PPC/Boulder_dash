#include "map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef __SPRITES__
#define __SPRITES__

#define TILE_S 12

typedef struct sprites_s
{
    ALLEGRO_BITMAP *sheet[2];

    ALLEGRO_BITMAP *dirt;
    ALLEGRO_BITMAP *boulder;
    ALLEGRO_BITMAP *diamond;
    ALLEGRO_BITMAP *wall;
    ALLEGRO_BITMAP *player; 
    ALLEGRO_BITMAP *blank;
    ALLEGRO_BITMAP *border;

    ALLEGRO_BITMAP *background;
} sprites_t;

void init_sprites(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp);

void initialize_sprites(sprites_t *sprites);

void destroy_sprites(sprites_t *sprites);

#endif