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

void update_player_speed(map_t *map, unsigned char *key)
{
    tile_t **mat = map->m[map->cur_m];
    int x = map->player_x;
    int y = map->player_y;
    tile_t *cur = &(mat[y][x]);

    cur->dy = 0;
    cur->dx = 0;

    if (key[ALLEGRO_KEY_LEFT] && x > 1)
    {
        tile_t *left = &(mat[y][x - 1]);
        tile_t *left_left = &(mat[y][x - 2]);

        if (test_walkable(left) ||
            (left->type == BOULDER && left_left->type == BLANK))
            cur->dx = -1;
    }
    else if (key[ALLEGRO_KEY_RIGHT] && x < map->width)
    {
        tile_t *right = &(mat[y][x + 1]);
        tile_t *right_right = &(mat[y][x + 2]);
        
        if (test_walkable(right) ||
            (right->type == BOULDER && right_right->type == BLANK))
            cur->dx = 1;
    }
    else if (key[ALLEGRO_KEY_UP] && y > 1)
    {
        tile_t *up = &(mat[y - 1][x]);
        if (test_walkable(up))
            cur->dy = -1;
    }
    else if (key[ALLEGRO_KEY_DOWN] && y < map->height)
    {
        tile_t *down = &(mat[y + 1][x]);
        if (test_walkable(down))
            cur->dy = 1;
    }

    if(cur->dy || cur->dx)
    {
        map->timer = MAP_TIMER;
        printf("%d %d\n", cur->dy, cur->dx);
    }
}

void update_boulder_speed(map_t *map, int y, int x)
{
    tile_t **mat = map->m[map->cur_m];
    tile_t *cur = &(mat[y][x]);
    tile_t *left = &(mat[y][x-1]);
    tile_t *right = &(mat[y][x+1]);
    tile_t *down = &(mat[y+1][x]);

    cur->dx = 0;
    cur->dy = 0;

    if (down->type == BLANK)
        cur->dy = 1;
    else if (left->type == PLAYER && left->dx == 1)
        cur->dx = 1;
    else if (right->type == PLAYER && right->dx == -1)
        cur->dx = -1;

    if (cur->dx || cur->dy)
        map->timer = MAP_TIMER;
}

void update_tiles_speed(map_t *map, unsigned char *key)
{
    tile_t **mat = map->m[map->cur_m];
    update_player_speed(map, key);

//    printf("%d %d\n", map->player_y, map->player_x);
//    if (map->m[map->cur_m][map->player_y][map->player_x].dx)
//        printf("%d\n", map->m[map->cur_m][map->player_y][map->player_x].dx);

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            switch(mat[i][j].type)
            {
                case BOULDER:
                    update_boulder_speed(map, i, j);
                    break;
            }
        }
}

void assign_tile(tile_t *d, tile_t *s)
{
    d->dx = s->dx;
    d->dy = s->dy;
    d->type = s->type;
}

void update_tiles_position(map_t *map)
{
    int next = (map->cur_m+1)%2;
    int cur = map->cur_m;

    tile_t **cur_mat = map->m[cur];
    tile_t **next_mat = map->m[next];
    
    initialize_map_matrix(next_mat, map->width, map->height);

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            tile_t *cur_tile = &(cur_mat[i][j]);
            if (cur_tile->dx || cur_tile->dy)
            {
                if (cur_tile->type == PLAYER)
                {
                    map->player_x = map->player_x + cur_tile->dx;
                    map->player_y = map->player_y + cur_tile->dy;
                }
                tile_t *next_tile = &(next_mat[i+cur_tile->dy][j+cur_tile->dx]);
                assign_tile(next_tile, cur_tile);
                next_tile->visited = 1;
            }
        }

    // Loop para tiles dinâmicos
    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            tile_t *cur_tile = &(cur_mat[i][j]);
            tile_t *next_tile = &(next_mat[i][j]);
            
            if (!cur_tile->dx && !cur_tile->dy && !next_tile->visited)
            {
                assign_tile(next_tile, cur_tile);
                next_tile->visited = 1;
            }
        }
}


void flip_map_matrix(map_t *map)
{
    map->cur_m = (map->cur_m + 1) % 2;
}

void update_map(map_t *map, unsigned char *key)
{
    update_tiles_position(map);
    flip_map_matrix(map);
    update_tiles_speed(map, key);
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