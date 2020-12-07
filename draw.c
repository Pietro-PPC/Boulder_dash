#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "map.h"
#include "sprites.h"
#include "draw.h"
#include "states.h"
#include <stdio.h>

void pre_draw(ALLEGRO_BITMAP *bitmap)
{
    al_set_target_bitmap(bitmap);
}

void post_draw(ALLEGRO_BITMAP *bitmap, ALLEGRO_DISPLAY *disp)
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(bitmap, 
                          0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, 
                          0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

    al_flip_display();
}

void draw_map(sprites_t *sprites, map_t *map)
{
    al_draw_bitmap(sprites->background, 0, 0, 0);

    int x, y;
    for (int i = 0; i < map->height; ++i)
    {
        for (int j = 0; j < map->width; ++j)
        {
            x = TILE_S*j;
            y = TILE_S*i;
            switch (map->m[i][j])
            {
                case BORDER: 
                    al_draw_bitmap(sprites->border, x, y, 0);
                    break;
                case BLANK:
                    al_draw_bitmap(sprites->blank, x, y, 0);
                    break;
                case DIRT:
                    al_draw_bitmap(sprites->dirt, x, y, 0);
                    break;
                case BOULDER:
                    al_draw_bitmap(sprites->boulder, x, y, 0);
                    break;
                case WALL:
                    al_draw_bitmap(sprites->wall, x, y, 0);
                    break;
                case DIAMOND:
                    al_draw_bitmap(sprites->diamond, x, y, 0);
                    break;
                case PLAYER:
                    al_draw_bitmap(sprites->player, x, y, 0);
                    break;
            }
        }
    }
}