#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "sprites.h"
#include "error_msg.h"

ALLEGRO_BITMAP *get_sprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sheet, x, y, w, h);
    test_init(sprite, "algum dos sprites");
    return sprite;
}

void initialize_sprites(sprites_t *sprites)
{
    sprites->sheet = NULL;
    sprites->border = NULL;
}

void init_sprites(sprites_t *sprites)
{
    sprites->sheet = al_load_bitmap("sprites/tileset.png");
    test_init(sprites->sheet, "tileset de sprites");

    sprites->border = get_sprite(sprites->sheet, 84, 0, 12, 12);
}

void destroy_sprites(sprites_t *sprites)
{
    al_destroy_bitmap(sprites->sheet);
    al_destroy_bitmap(sprites->border);
}