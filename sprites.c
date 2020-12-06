#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "sprites.h"
#include "error_msg.h"
#include "map.h"

ALLEGRO_BITMAP *get_sprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sheet, x, y, w, h);
    test_init(sprite, "algum dos sprites");
    return sprite;
}

void initialize_sprites(sprites_t *sprites)
{
    sprites->sheet[0] = NULL;
    sprites->sheet[1] = NULL;

    sprites->blank = NULL;
    sprites->dirt = NULL;
    sprites->border = NULL;
    sprites->boulder = NULL;
    sprites->diamond = NULL;
    sprites->player = NULL;
    sprites->wall = NULL;

    sprites->background = NULL;
}

void init_background(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp)
{
    sprites->background = al_create_bitmap(map->width*TILE_S, map->height*TILE_S);
    test_init(sprites->background, "sprite do fundo");

    al_set_target_bitmap(sprites->background);

    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
        {
            al_draw_bitmap(sprites->blank, j*TILE_S, i*TILE_S, 0);
        }
    al_set_target_backbuffer(disp);
}

void init_sprites(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp)
{
    sprites->sheet[0] = al_load_bitmap("sprites/tileset.png");
    test_init(sprites->sheet[0], "tileset de sprites");

    sprites->sheet[1] = al_load_bitmap("sprites/Background-Ingame.png");
    test_init(sprites->sheet[1], "imagem de fundo");

    sprites->wall = get_sprite(sprites->sheet[0], 84, 0, TILE_S, TILE_S);
    sprites->dirt = get_sprite(sprites->sheet[0], 12, 12, TILE_S, TILE_S);
    sprites->boulder = get_sprite(sprites->sheet[0], 0, 204, TILE_S, TILE_S);
    sprites->diamond = get_sprite(sprites->sheet[0], 36, 144, TILE_S, TILE_S);
    sprites->border = get_sprite(sprites->sheet[0], 228, 36, TILE_S, TILE_S);
    sprites->player = get_sprite(sprites->sheet[0], 24, 144, TILE_S, TILE_S);
    
    sprites->blank = get_sprite(sprites->sheet[1], 0, 0, TILE_S, TILE_S);
    
    init_background(sprites, map, disp);
}

void destroy_sprites(sprites_t *sprites)
{
    al_destroy_bitmap(sprites->sheet[0]);
    al_destroy_bitmap(sprites->sheet[1]);
    al_destroy_bitmap(sprites->border);
    al_destroy_bitmap(sprites->background);
    al_destroy_bitmap(sprites->blank);
    al_destroy_bitmap(sprites->boulder);
    al_destroy_bitmap(sprites->diamond);
    al_destroy_bitmap(sprites->dirt);
    al_destroy_bitmap(sprites->player);
    al_destroy_bitmap(sprites->wall);
}