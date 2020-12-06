#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef __SPRITES__
#define __SPRITES__

typedef struct sprites_s
{
    ALLEGRO_BITMAP* sheet;

//    ALLEGRO_BITMAP* ground;

//    ALLEGRO_BITMAP* boulder;

//    ALLEGRO_BITMAP* diamond;

//    ALLEGRO_BITMAP* wall;

    ALLEGRO_BITMAP* border;
} sprites_t;

void init_sprites(sprites_t *sprites);

void initialize_sprites(sprites_t *sprites);

void destroy_sprites(sprites_t *sprites);

#endif