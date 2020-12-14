#include <allegro5/allegro.h>
#include <stdio.h>
#include "map.h"
#include "game.h"
#include "update.h"
#include "audio.h"

int test_falls(tile_t *t)
// testa se tile é de um tipo que cai
{
    char c = t->type;
    return (c == DIAMOND ||
            c == BOULDER);
}

void update_player_speed(game_t *game, unsigned char *key)
// atualiza velocidade do jogador
{
    map_t *map = &(game->map);
    tile_t **mat = map->m[map->cur_m];
    int x = map->player_x;
    int y = map->player_y;
    tile_t *cur = &(mat[y][x]);

    cur->dy = 0;
    cur->dx = 0;

    if (key[ALLEGRO_KEY_LEFT] && x > 1)
    {
        if ((test_walkable(&(mat[y][x-1])) ||                               // jogador pode atravessar o tile da esquerda?
            (mat[y][x-1].type == BOULDER && mat[y][x-2].type == BLANK) ||   // há uma pedra que pode ser empurrada na esquerda?
            (mat[y][x-1].type == EXIT && map->open_exit)) &&                // há alguma saída aberta na esquerda?
            !( test_falls(&(mat[y-1][x-1])) && mat[y][x-1].type == BLANK) ) // não há nada caindo no bloco da esquerda?
            cur->dx = -1;
    }
    else if (key[ALLEGRO_KEY_RIGHT] && x < map->width)
    {
        // análogo ao if da tecla para a esquerda
        if ((test_walkable(&(mat[y][x+1])) ||
            (mat[y][x+1].type == BOULDER && mat[y][x+2].type == BLANK) ||
            (mat[y][x+1].type == EXIT && map->open_exit)) &&
            !( test_falls(&(mat[y-1][x+1])) && mat[y][x+1].type == BLANK ))
            cur->dx = 1;
    }
    else if (key[ALLEGRO_KEY_UP] && y > 1)
    {
        if ((test_walkable(&(mat[y-1][x])) ||                 // jogador pode atravessar o tile de cima?
            (mat[y-1][x].type == EXIT && map->open_exit)) &&  // há uma saída em cima?
            !mat[y-1][x].visited)                             // o bloco de cima não será visitado por outro bloco?
            cur->dy = -1;
    }
    else if (key[ALLEGRO_KEY_DOWN] && y < map->height)
    {
        // análogo ao if da tecla para cima
        if ((test_walkable(&(mat[y+1][x])) ||
            (mat[y+1][x].type == EXIT && map->open_exit)) && 
            !mat[y+1][x].visited)
            cur->dy = 1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;
    // se jogador encontrou a saída aberta
    if (mat[y+cur->dy][x+cur->dx].type == EXIT)
    {
        game->n_plays.victory = 1;
        game->endgame = 1;
        game->score += game->time;
    }
    // se jogador encontrou parede falsa
    if (mat[y+cur->dy][x+cur->dx].type == FAKE_WALL)
        game->n_plays.wow = 1;

    // se jogador se mexeu, tocarão dois sons de passo
    game->n_plays.step = 0;
    if(cur->dy || cur->dx)
    {
        game->n_plays.step = 2;
        map->timer = MAP_TIMER;
    }
}

void update_boulder_speed(game_t *game, int y, int x)
// atualiza velocidade de pedra
{
    map_t *map = &(game->map);
    tile_t **mat = map->m[map->cur_m];
    tile_t *cur = &(mat[y][x]);
    int prev_dy = cur->dy;

    cur->dx = 0;
    cur->dy = 0;

    // teste se pedra pode cair
    if (mat[y+1][x].type == BLANK || (mat[y+1][x].type == PLAYER && prev_dy > 0))
        cur->dy = 1;
    // teste se foi empurrada por um player
    else if (mat[y][x-1].type == PLAYER && mat[y][x-1].dx == 1 &&
            !mat[y][x+1].visited)
        cur->dx = 1;
    // teste se foi empurrade por um player do outro lado
    else if (mat[y][x+1].type == PLAYER && mat[y][x+1].dx == -1 &&
            !mat[y][x-1].visited)
        cur->dx = -1;
    // teste se está no topo de uma pilha de diamantes/pedras
    else if (test_falls(&(mat[y+1][x])) && !(test_falls(&(mat[y-1][x]))))
    {
        // teste se rola para a esquerda ou direita
        if (mat[y][x-1].type == BLANK && mat[y+1][x-1].type == BLANK &&
            !mat[y][x-1].visited)
            cur->dx = -1;
        else if (mat[y][x+1].type == BLANK && mat[y+1][x+1].type == BLANK &&
                !mat[y][x+1].visited)
            cur->dx = 1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;

    // se há movimento, temporizador recomeça
    if (cur->dx || cur->dy)
        map->timer = MAP_TIMER;
    // se deixou de cair, toca som
    if (prev_dy && !cur->dy)
        game->n_plays.boulder_hit = 1;
}

void update_diamond_speed(game_t *game, int y, int x)
// atualiza velocidade do diamante
{
    map_t *map = &(game->map);
    tile_t **mat = map->m[map->cur_m];
    tile_t *cur = &(mat[y][x]);
    int prev_dy = cur->dy;

    cur->dx = 0;
    cur->dy = 0;
    // Testa se vai cair ou matar o player
    if (mat[y+1][x].type == BLANK || (mat[y+1][x].type == PLAYER && prev_dy > 0))
        cur->dy = 1;
    // testa se está no topo de uma pilha de diamantes/pedras
    else if (test_falls(&(mat[y+1][x])) && !test_falls(&(mat[y-1][x])))
    {
        // teste se rola para esquerda ou direita
        if (mat[y][x+1].type == BLANK && mat[y+1][x+1].type == BLANK &&
            !mat[y][x+1].visited)
            cur->dx = 1;
        else if (mat[y][x-1].type == BLANK && mat[y+1][x-1].type == BLANK &&
                 !mat[y][x-1].visited)
            cur->dx = -1;
    }

    mat[y+cur->dy][x+cur->dx].visited = 1;

    // atualiza temporizador do jogo
    if (cur->dx || cur->dy)
        map->timer = MAP_TIMER;
    // se há parada brusca, áudio será tocado
    if (prev_dy && !cur->dy)
        game->n_plays.boulder_hit = 1;
}

void update_tiles_speed(game_t *game, unsigned char *key)
// atualiza velocidade dos tiles do jogo
{
    map_t *map = &(game->map);
    tile_t **mat = map->m[map->cur_m];

    // inicializa estado de visitado dos tiles
    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
            mat[i][j].visited = 0;

    // atualiza velocidade do jogador
    update_player_speed(game, key);

    // atualiza velocidade de pedras e diamantes
    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            switch(mat[i][j].type)
            {
                case BOULDER:
                    update_boulder_speed(game, i, j);
                    break;
                case DIAMOND:
                    update_diamond_speed(game, i, j);
                    break;
            }
        }
}

void update_tiles_position(map_t *map)
// atualiza posição dos tiles
{
    int next = (map->cur_m+1)%2;
    int cur = map->cur_m;

    tile_t **cur_mat = map->m[cur];
    tile_t **next_mat = map->m[next];
    
    // inicializa próxima matriz
    initialize_map_matrix(next_mat, map->width, map->height);

    // loop para tiles que se mexem
    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
        {
            tile_t *cur_tile = &(cur_mat[i][j]);
            if (cur_tile->dx || cur_tile->dy)
            {
                // atualiza variáveis do player caso ele tenha se mexido
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

    // Loop para tiles parados
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
// alterna matriz atual
{
    map->cur_m = (map->cur_m + 1) % 2;
}

int test_player_died(map_t *map)
// testa se jogador morreu
{
    tile_t **mat = map->m[map->cur_m];
    int py = map->player_y;
    int px = map->player_x;
    return (!mat[py][px].dx && mat[py][px].dy != 1 && mat[py-1][px].dy == 1);
}

int disappears(tile_t *t)
// testa se é um tile que desaparece
{
    char c = t->type;
    return (c == DIAMOND ||
            c == DIRT ||
            c == FAKE_WALL);
}

void reset_disappear_state(map_t *map)
// recomeça estado de desaparecer do tile
{
    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
            map->m[map->cur_m][i][j].disappear = 0;
}

void update_disappear_state(game_t *game)
// atualiza estado de desaparecer dos tiles
{
    tile_t **mat = game->map.m[game->map.cur_m];
    int px = game->map.player_x;
    int py = game->map.player_y;
    int dx = mat[py][px].dx;
    int dy = mat[py][px].dy;

    tile_t* t = &(mat[py+dy][px+dx]);

    // se jogador se mexeu para um tile que desaparece e 
    // ele não está se mexendo para outro lugar, ele desaparece
    if ((dx || dy) && disappears(t) && !(t->dy))
    {
        // se for um diamante, conta de diamantes aumenta
        if (t->type == DIAMOND)
        {
            game->diamonds_got++;
            game->score += 10;
            game->n_plays.diamond = 1;
        }
        // se for uma parede falsa, jogador ganha 50 pontos
        else if(t->type == FAKE_WALL)
            game->score += 50;
        t->disappear = 1;
    }
}

void update_game(game_t *game, unsigned char *key)
// atualiza posições das entidades do jogo, pontuação e quais audios
// serão tocados da próxia vez
{
    update_tiles_position(&(game->map));
    flip_map_matrix(&(game->map));

    reset_disappear_state(&(game->map));
    update_tiles_speed(game, key);
    update_disappear_state(game);
    
    if (game->diamonds_got >= DIAMOND_WIN && !game->map.open_exit)
    {
        game->map.open_exit = 1;
        game->n_plays.open_door = 1;
    }
    // decrementa vida se jogador tiver morrido
    if (test_player_died(&(game->map)))
        game->lives--;
}

void explode_player(map_t *map)
// se player morreu, substitui 9 tiles não borders em volta por explosões.
{
    tile_t **mat = map->m[(map->cur_m+1)%2];
    int x = map->player_x;
    int y = map->player_y;

    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            if (mat[y + i][x + j].type != BORDER)
                initialize_tile(&(mat[y + i][x + j]), EXPLOSION);
    
    map->timer = MAP_TIMER;    
}

void end_game(game_t *game)
// atualiza posição determinada anteriormente e explode o player
{
    update_tiles_position(&(game->map));
    explode_player(&(game->map));
    flip_map_matrix(&(game->map));
}