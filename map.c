/* ideia: detectar e avisar erro no mapa */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_msg.h"
#include "map.h"
#include "sprites.h"

void initialize_map(map_t *map)
{
    map->width = 0;
    map->height = 0;
    map->timer = 0;
    map->m = NULL;
}

void allocate_map(map_t *map)
{
    if (!map->width || !map->height)
        fatal_error("Mapa sem dimensões");

    map->m = malloc(map->height * sizeof(tile_t *));
    if (!map)
        fatal_error("Falha ao alocar mapa");

    // +1 para armazenar o \0 ao final
    map->m[0] = malloc(map->width * map->height * sizeof(tile_t) + 1); 
    if (!map->m[0])
        fatal_error("Falha ao alocar mapa");

    for (int i = 1; i < map->height; ++i)
        map->m[i] = map->m[0] + i * map->width;
}

void initialize_tile(tile_t *t)
{
    t->dx = 0;
    t->dy = 0;
    t->type = BLANK;
}

void read_map(map_t *map)
/* ATUALIZAR: lidar com mais de um player ou 0 players no campo */
{
    FILE *f;
    char filename[101] = "levelmaps/";
    strcat(filename, LEVELFILE);

    f = fopen(filename, "r");
    if (!f)
        fatal_error("Falha ao abrir mapa");

    // lê linha com largura e altura e pula para a próxima
    fscanf(f, "%d %d", &(map->width), &(map->height));
    while(getc(f) != '\n' && !feof(f));
    allocate_map(map);

    // Inicializa mapa com valores em branco
    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
            initialize_tile(&(map->m[i][j]));

    for (int i = 0; i < map->height && !feof(f); ++i)
    {
        for (int j = 0; j < map->width; ++j)
        {
            map->m[i][j].type = getc(f);
            if (map->m[i][j].type == PLAYER)
            {
                map->player_y = i;
                map->player_x = j;
            }
        }
        // itera para proxima linha ou fim de arquivo
        while(getc(f) != '\n' && !feof(f));
    }
}

/*
int *x = &(map->player_x);
    int *y = &(map->player_y);
    if (key[ALLEGRO_KEY_LEFT])
    {
        map->m[*y][*x - 1].type = PLAYER;
        map->m[*y][*x].type = BLANK;
        (*x)--;
        map->timer = 12;
    }
    else if (key[ALLEGRO_KEY_RIGHT])
    {
        map->m[*y][*x + 1].type = PLAYER;
        map->m[*y][*x].type = BLANK;
        (*x)++;
        map->timer = 12;
    }
    else if (key[ALLEGRO_KEY_UP])
    {
        map->m[*y - 1][*x].type = PLAYER;
        map->m[*y][*x].type = BLANK;
        (*y)--;
        map->timer = 12;
    }
    else if (key[ALLEGRO_KEY_DOWN])
    {
        map->m[*y + 1][*x].type = PLAYER;
        map->m[*y][*x].type = BLANK;
        (*y)++;
        map->timer = 12;
    }
*/
int test_solid(char c)
{
    return (c == BOULDER ||
            c == WALL ||
            c == BORDER);
}


void update_player_speed(map_t *map, unsigned char *key)
{
    int *x = &(map->player_x);
    int *y = &(map->player_y);
    if (key[ALLEGRO_KEY_LEFT])
    {
        char left_tile = map->m[*y][*x - 1].type;
        if (!test_solid(left_tile))
        {
            map->m[*y][*x].dx = -1;
            map->timer = 12;
        }
        else if (left_tile == BOULDER && map->m[*y][*x-2].type == BLANK)
        {
            map->m[*y][*x].dx = -1;
            map->timer = 12;
        }
        else     
            map->m[*y][*x].dx = 0;
    }
    else if (key[ALLEGRO_KEY_RIGHT])
    {
        char right_tile = map->m[*y][*x + 1].type;
        if (!test_solid(right_tile))
        {
            map->m[*y][*x].dx = 1;
            map->timer = 12;
        }
        else if (right_tile == BOULDER && map->m[*y][*x+2].type == BLANK)
        {
            map->m[*y][*x].dx = 1;
            map->timer = 12;
        }
        else
            map->m[*y][*x].dx = 0;
    }
    else if (key[ALLEGRO_KEY_UP])
    {
        char up_tile = map->m[*y - 1][*x].type;
        if (!test_solid(up_tile))
            map->m[*y][*x].dy = -1;
        else
            map->m[*y][*x].dy = 0;
        map->timer = 12;
    }
    else if (key[ALLEGRO_KEY_DOWN])
    {
        char down_tile = map->m[*y + 1][*x].type;
        if (!test_solid(down_tile))
            map->m[*y][*x].dy = 1;
        else
            map->m[*y][*x].dy = 0;
        map->timer = 12;
    }
}

void update_boulder_speed(map_t *map, int i, int j)
{
    if (map->m[i+1][j].type == BLANK)
    {
        map->m[i][j].dy = 1;
        map->timer = 12;
    }
    else if (map->m[i][j-1].type == PLAYER && map->m[i][j-1].dx == 1)
    {
        map->m[i][j].dx = 1;
        map->timer = 12;
    }
    else if (map->m[i][j+1].type == PLAYER && map->m[i][j+1].dx == -1)
    {
        map->m[i][j].dx = -1;
        map->timer = 12;
    }
    else
    {
        map->m[i][j].dy = 0;
        map->m[i][j].dx = 0;
    }
}

void update_tiles_speed(map_t *map, unsigned char *key)
{
    update_player_speed(map, key);
    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
        {
            switch(map->m[i][j].type)
            {
                case BOULDER:
                    update_boulder_speed(map, i, j);
                    break;
                
            }
        }
}

void update_tiles_position(map_t *map)
{
    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
        {
            int *px = &(map->player_x);
            int *py = &(map->player_y);
            tile_t *curr = &(map->m[i][j]);

            if (curr->dx > 0)
            {
                map->m[i][j+1].type = curr->type;
                if (curr->type == PLAYER)
                    (*px)++;
                initialize_tile(curr);
            }
            else if (curr->dx < 0)
            {
                map->m[i][j-1].type = curr->type;
                if (curr->type == PLAYER)
                    (*px)--;
                initialize_tile(curr);
            }
            else if (curr->dy > 0)
            {
                map->m[i+1][j].type = curr->type;
                if (curr->type == PLAYER)
                    (*py)++;
                initialize_tile(curr);
            }
            else if (curr->dy < 0)
            {
                map->m[i-1][j].type = curr->type;
                if (curr->type == PLAYER)
                    (*py)--;
                initialize_tile(curr);
            }
        }
}

void update_map(map_t *map, unsigned char *key)
{
    update_tiles_position(map);
    update_tiles_speed(map, key);
    //printf("oi\n");
}

void destroy_map(map_t *map)
{
    for (int i = 1; i < map->height; ++i)
        map->m[i] = NULL;
    
    if (map->m[0])
        free(map->m[0]);
    map->m[0] = NULL;

    if (map->m)
        free(map->m);
    map->m = NULL;
}