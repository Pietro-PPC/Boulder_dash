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
    map->cur_m = 0;
    map->m[0] = NULL;
    map->m[1] = NULL;
}

void initialize_tile_blank(tile_t *t)
{
    t->dx = 0;
    t->dy = 0;
    t->type = BLANK;
    t->visited = 0;
}
void initialize_tile_border(tile_t *t)
{
    t->dx = 0;
    t->dy = 0;
    t->type = BORDER;
    t->visited = 0;
}

void initialize_map_matrix(tile_t **m, int w, int h)
{
    for (int i = 0; i < h+2; ++i)
        for (int j = 0; j < w+2; ++j)
            if (i == 0 || i > h || j == 0 || j > w)
                initialize_tile_border(&(m[i][j]));
            else
                initialize_tile_blank(&(m[i][j]));
}

void allocate_map(tile_t ***map, int w, int h)
{
    // Alocar ponteiros para linhas
    (*map) = malloc((h+2) * sizeof(tile_t *));
    if (!*map)
        fatal_error("Falha ao alocar mapa");
    
    // Alocar matriz 
    (*map)[0] = malloc((w+2) * (h+2) * sizeof(tile_t)); 
    if (!(*map)[0])
        fatal_error("Falha ao alocar mapa");
    
    // Assinalar ponteiros às linhas
    for (int i = 1; i < (h+2); ++i)
        (*map)[i] = (*map)[0] + i * (w+2);
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

    tile_t **initMap = map->m[map->cur_m];
    for (int i = 1; i <= map->height && !feof(f); ++i)
    {
        for (int j = 1; j <= map->width; ++j)
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

int test_walkable(tile_t *t)
{
    char c = t->type;
    return !(c == BOULDER ||
             c == WALL ||
             c == BORDER);
}

int test_solid(tile_t *t)
{
    char c = t->type;
    return (c == BOULDER ||
            c == WALL || 
            c == BORDER || 
            c == DIRT);
}

void assign_tile(tile_t *d, tile_t *s)
{
    d->dx = s->dx;
    d->dy = s->dy;
    d->type = s->type;
}

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