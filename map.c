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
    map->m[0] = NULL;
    map->m[1] = NULL;
}

void initialize_tile(tile_t *t)
{
    t->dx = 0;
    t->dy = 0;
    t->type = BLANK;
}

void initialize_map_matrix(tile_t **m, int w, int h)
{
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            initialize_tile(&(m[i][j]));
}

void allocate_map(tile_t ***map, int w, int h)
{
    // Alocar ponteiros para linhas
    (*map) = malloc(h * sizeof(tile_t *));
    if (!*map)
        fatal_error("Falha ao alocar mapa");
    
    // Alocar matriz 
    (*map)[0] = malloc(w * h * sizeof(tile_t)); 
    if (!(*map)[0])
        fatal_error("Falha ao alocar mapa");
    
    // Assinalar ponteiros aos 
    for (int i = 1; i < h; ++i)
        (*map)[i] = (*map)[0] + i * w;
}

void read_map(map_t *map)
/* ATUALIZAR: lidar com mais de um player ou 0 players no campo */
{
    FILE *f;
    char filename[101] = "levelmaps/";
    strcat(filename, LEVELFILE);

    // Abre arquivo do nível
    f = fopen(filename, "r");
    if (!f)
        fatal_error("Falha ao abrir mapa");

    // lê linha com largura e altura e pula para a próxima linha
    fscanf(f, "%d %d", &(map->width), &(map->height));
    while(getc(f) != '\n' && !feof(f));

    // Testa dimensões do mapa e aloca caso sejam não nulas
    if (!map->width || !map->height)
        fatal_error("Mapa sem dimensões");
    allocate_map(&(map->m[0]), map->width, map->height);
    allocate_map(&(map->m[1]), map->width, map->height);

    // Inicializa mapas com valores em branco
    initialize_map_matrix(map->m[0], map->width, map->height);
    initialize_map_matrix(map->m[1], map->width, map->height);

    tile_t **initMap = map->m[0];
    for (int i = 0; i < map->height && !feof(f); ++i)
    {
        for (int j = 0; j < map->width; ++j)
        {
            initMap[i][j].type = getc(f);
            if (initMap[i][j].type == PLAYER)
            {
                map->player_y = i;
                map->player_x = j;
            }
        }
        // itera para proxima linha ou fim de arquivo
        while(getc(f) != '\n' && !feof(f));
    }
}

int test_solid(char c)
{
    return (c == BOULDER ||
            c == WALL ||
            c == BORDER);
}

/*arrumar
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
    else
    {
        map->m[*y][*x].dy = 0;
        map->m[*y][*x].dx = 0;
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
            map->m[i][j].visited = 0;

    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
        {
            tile_t *curr = &(map->m[i][j]);
            if (curr->visited)
                continue;
            curr->visited = 1;

            int *px = &(map->player_x);
            int *py = &(map->player_y);

            if (curr->dx > 0)
            {
                if (map->m[i][j+1].type == BOULDER)
                {
                    if (map->m[i][j+1].dx == 1)
                    {
                        map->m[i][j+2].type = BOULDER;
                        map->m[i][j+2].visited = 1;
                    }
                    else if (map->m[i][j+1].dy == 1)
                    {
                        map->m[i+1][j+1].type = BOULDER;
                        map->m[i+1][j+1].visited = 1;
                    }
                }
                
                map->m[i][j+1].visited = 1;
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

*/

void destroy_map_matrix(tile_t ***m, int w, int h)
{
    for (int i = 1; i < h; ++i)
        (*m)[i] = NULL;
    
    if ((*m)[0])
        free((*m)[0]);
    (*m)[0] = NULL;

    if ((*m))
        free((*m));
    (*m) = NULL;
}

void destroy_map(map_t *map)
{
    destroy_map_matrix(&(map->m[0]), map->width, map->height);
    destroy_map_matrix(&(map->m[1]), map->width, map->height);
}