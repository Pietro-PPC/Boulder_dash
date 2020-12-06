#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "map.h"
#include "sprites.h"
#include "draw.h"

void draw_map(sprites_t *sprites, map_t *map)
{
    for (int i = 0; i < map->height; ++i)
    {
        for (int j = 0; j < map->width; ++j)
        {
            switch (map->m[i][j])
            {
                case BORDER: 
                    al_draw_bitmap(sprites->border, 12*j, 12*i, 0);
                    break;
            }
        }
    }
}