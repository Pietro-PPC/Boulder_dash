#include <allegro5/allegro.h>
#include <stdio.h>
#include "map.h"
#include "game.h"

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
        if ((test_walkable(&(mat[y][x-1])) ||
            (mat[y][x-1].type == BOULDER && mat[y][x-2].type == BLANK)) &&
            !mat[y][x-1].visited)
            cur->dx = -1;
    }
    else if (key[ALLEGRO_KEY_RIGHT] && x < map->width)
    {
        if ((test_walkable(&(mat[y][x+1])) ||
            (mat[y][x+1].type == BOULDER && mat[y][x+2].type == BLANK)) &&
            !mat[y][x+1].visited)
            cur->dx = 1;
    }
    else if (key[ALLEGRO_KEY_UP] && y > 1)
    {
        if (test_walkable(&(mat[y-1][x])) && 
            !mat[y-1][x].visited)
            cur->dy = -1;
    }
    else if (key[ALLEGRO_KEY_DOWN] && y < map->height)
    {
        if (test_walkable(&(mat[y+1][x])) && 
            !mat[y+1][x].visited)
            cur->dy = 1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;

    if(cur->dy || cur->dx)
        map->timer = MAP_TIMER;
}

int test_falls(tile_t *t)
{
    char c = t->type;
    return (c == DIAMOND ||
            c == BOULDER);
}

void update_boulder_speed(map_t *map, int y, int x)
{
    tile_t **mat = map->m[map->cur_m];
    tile_t *cur = &(mat[y][x]);
    int prev_dy = cur->dy;

    cur->dx = 0;
    cur->dy = 0;

    if (mat[y+1][x].type == BLANK || (mat[y+1][x].type == PLAYER && prev_dy > 0))
        cur->dy = 1;
    else if (mat[y][x-1].type == PLAYER && mat[y][x-1].dx == 1 &&
            !mat[y][x+1].visited)
        cur->dx = 1;
    else if (mat[y][x+1].type == PLAYER && mat[y][x+1].dx == -1 &&
            !mat[y][x-1].visited)
        cur->dx = -1;
    else if (test_falls(&(mat[y+1][x])) && !test_falls(&(mat[y-1][x])) )
    {
        if (mat[y][x-1].type == BLANK && mat[y+1][x-1].type == BLANK &&
            !mat[y][x-1].visited)
            cur->dx = -1;
        else if (mat[y][x+1].type == BLANK && mat[y+1][x+1].type == BLANK &&
                !mat[y][x+1].visited)
            cur->dx = 1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;

    if (cur->dx || cur->dy)
        map->timer = MAP_TIMER;
}

void update_diamond_speed(map_t *map, int y, int x)
{
    tile_t **mat = map->m[map->cur_m];
    tile_t *cur = &(mat[y][x]);
    int prev_dy = cur->dy;

    cur->dx = 0;
    cur->dy = 0;
    if (mat[y+1][x].type == BLANK || (mat[y+1][x].type == PLAYER && prev_dy > 0))
        cur->dy = 1;
    else if (test_falls(&(mat[y+1][x])) && !test_falls(&(mat[y-1][x])))
    {
        if (mat[y][x+1].type == BLANK && mat[y+1][x+1].type == BLANK &&
            !mat[y][x+1].visited)
            cur->dx = 1;
        else if (mat[y][x-1].type == BLANK && mat[y-1][x-1].type == BLANK &&
                 !mat[y][x-1].visited)
            cur->dx = -1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;

    if (cur->dx || cur->dy)
        map->timer = MAP_TIMER;
}

void update_tiles_speed(map_t *map, unsigned char *key)
{
    tile_t **mat = map->m[map->cur_m];

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
            mat[i][j].visited = 0;

    update_player_speed(map, key);

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            switch(mat[i][j].type)
            {
                case BOULDER:
                    update_boulder_speed(map, i, j);
                    break;
                case DIAMOND:
                    update_diamond_speed(map, i, j);
                    break;
            }
        }
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

int test_player_died(map_t *map)
{
    tile_t **mat = map->m[map->cur_m];
    int py = map->player_y;
    int px = map->player_x;
    return (!mat[py][px].dx && mat[py][px].dy != 1 && mat[py-1][px].dy == 1);
}

void update_game(game_t *game, unsigned char *key)
{
    update_tiles_position(&(game->map));
    flip_map_matrix(&(game->map));
    update_tiles_speed(&(game->map), key);
    if (test_player_died(&(game->map)))
        game->lives--;
}

void explode_player(map_t *map)
{
    tile_t **mat = map->m[(map->cur_m+1)%2];
    int x = map->player_x;
    int y = map->player_y;

    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            if (mat[y + i][x + j].type != BORDER)
                initialize_tile_explosion(&(mat[y + i][x + j]));
    
    map->timer = MAP_TIMER;    
}

void end_game(game_t *game)
{
    update_tiles_position(&(game->map));
    explode_player(&(game->map));
    flip_map_matrix(&(game->map));
}