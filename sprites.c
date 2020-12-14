#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "sprites.h"
#include "error_msg.h"
#include "map.h"

ALLEGRO_BITMAP *get_sprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h)
// carrega sprite de sheet e aborta em caso de erro
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sheet, x, y, w, h);
    test_init(sprite, "algum dos sprites");
    return sprite;
}

void initialize_sprites(sprites_t *sprites)
// inicializa variáveis dos sprites
{
    sprites->sheet[0] = NULL;
    sprites->sheet[1] = NULL;

    sprites->hud = NULL;

    sprites->blank = NULL;
    sprites->dirt = NULL;
    sprites->border = NULL;
    sprites->wall = NULL;

    sprites->exit.closed = NULL;
    for (int i = 0; i < N_TRANSITION; ++i)
        sprites->exit.open[i] = NULL;

    for (int i = 0; i < N_TRANSITION; ++i)
        sprites->explosion.explosion[i] = NULL;

    sprites->boulder.stop = NULL;
    for (int i = 0; i < N_TRANSITION; ++i)
        sprites->boulder.roll[i] = NULL;

    sprites->diamond.stop = NULL;
    for(int i = 0; i < N_TRANSITION; ++i)
    {
        sprites->diamond.fall[i] = NULL;
        sprites->diamond.disappear[i] = NULL;
    }

    sprites->player.stop = NULL;
    for (int i = 0; i < N_TRANSITION; ++i)
    {
        sprites->player.run_left[i] = NULL;
        sprites->player.run_right[i] = NULL;
    }

    sprites->background = NULL;
}

void init_background(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp)
// cria sprite para o fundo, onde não há nenhum objeto.
{
    // cria sprite de fundo com altura e largura do tabuleiro
    sprites->background = al_create_bitmap(map->width*TILE_S, map->height*TILE_S);
    test_init(sprites->background, "sprite do fundo");

    al_set_target_bitmap(sprites->background);

    // desenha no sprite criado
    for (int i = 0; i < map->height; ++i)
        for (int j = 0; j < map->width; ++j)
            al_draw_bitmap(sprites->blank, j*TILE_S, i*TILE_S, 0);

    al_set_target_backbuffer(disp);
}

void init_player_sprites(sprites_t *sprites)
// carrega sprites do jogador
{
    sprites->player.stop = get_sprite(sprites->sheet[0], 24, 144, TILE_S, TILE_S);
    for(int i = 0; i < N_TRANSITION; ++i)
    {
        sprites->player.run_right[i] = get_sprite(sprites->sheet[0], 72 + TILE_S*i, 156, TILE_S, TILE_S);
        sprites->player.run_left[i] = get_sprite(sprites->sheet[0], 60 - TILE_S*i, 156, TILE_S, TILE_S);
    }
}

void init_diamond_sprites(sprites_t *sprites)
// carrega sprites do diamante
{
    sprites->diamond.stop = get_sprite(sprites->sheet[0], 36, 144, TILE_S, TILE_S);
    sprites->diamond.fall[0] = get_sprite(sprites->sheet[0], 36, 144, TILE_S, TILE_S);
    sprites->diamond.disappear[0] = get_sprite(sprites->sheet[0], 36, 144, TILE_S, TILE_S);
    for (int i = 1; i < N_TRANSITION; ++i)
    {
        sprites->diamond.fall[i] = get_sprite(sprites->sheet[0], 36 + i*TILE_S, 144, TILE_S, TILE_S);
        sprites->diamond.disappear[i] = get_sprite(sprites->sheet[0], 0 + (i-1)*TILE_S, 192, TILE_S, TILE_S);
    }
}

void init_boulder_sprites(sprites_t *sprites)
// carrega sprites de pedras
{
    sprites->boulder.stop = get_sprite(sprites->sheet[0], 0, 204, TILE_S, TILE_S);
    sprites->boulder.roll[0] = get_sprite(sprites->sheet[0], 0, 204, TILE_S, TILE_S);
    for (int i = 1; i < N_TRANSITION; ++i)
        sprites->boulder.roll[i] = get_sprite(sprites->sheet[0], 0 + i*TILE_S, 204, TILE_S, TILE_S);
}

void init_explosion_sprites(sprites_t *sprites)
// carrega sprites de explosão
{
    for (int i = 0; i < N_TRANSITION; ++i)
        sprites->explosion.explosion[i] = get_sprite(sprites->sheet[0], i*TILE_S, 180, TILE_S, TILE_S);
}

void init_exit_sprites(sprites_t *sprites)
// carrega sprites da porta de saída
{
    sprites->exit.closed = get_sprite(sprites->sheet[0], 108, 144, TILE_S, TILE_S);
    for (int i = 0; i < N_TRANSITION; ++i)
        sprites->exit.open[i] = get_sprite(sprites->sheet[0], 120 + i*TILE_S, 144, TILE_S, TILE_S);
}

void init_sprites(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp)
// carrega todos os sprites
{
    // carrega e imagens de arquivos
    sprites->sheet[0] = al_load_bitmap("resources/sprites/tileset.png");
    test_init(sprites->sheet[0], "tileset de sprites");

    sprites->sheet[1] = al_load_bitmap("resources/sprites/Background-Ingame.png");
    test_init(sprites->sheet[1], "imagem de fundo");

    sprites->hud = al_load_bitmap("resources/sprites/HUD.png");
    test_init(sprites->hud, "HUD");


    // carrega sprites de um estado das spritesheets
    sprites->wall = get_sprite(sprites->sheet[0], 84, 0, TILE_S, TILE_S);
    sprites->dirt = get_sprite(sprites->sheet[0], 12, 12, TILE_S, TILE_S);
    sprites->border = get_sprite(sprites->sheet[0], 228, 36, TILE_S, TILE_S);
    sprites->blank = get_sprite(sprites->sheet[1], 0, 0, TILE_S, TILE_S);
    
    // carrega sprites de mais de um estado das spritesheets
    init_exit_sprites(sprites);
    init_explosion_sprites(sprites);
    init_boulder_sprites(sprites);
    init_diamond_sprites(sprites);
    init_player_sprites(sprites);
    
    init_background(sprites, map, disp);
}

void destroy_sprites(sprites_t *sprites)
// desaloca todos os sprites carregados
{
    // desaloca spritesheets e sprites com um estado
    al_destroy_bitmap(sprites->sheet[0]);
    al_destroy_bitmap(sprites->sheet[1]);
    al_destroy_bitmap(sprites->hud);
    al_destroy_bitmap(sprites->border);
    al_destroy_bitmap(sprites->background);
    al_destroy_bitmap(sprites->blank);
    al_destroy_bitmap(sprites->dirt);
    al_destroy_bitmap(sprites->wall);

    // desaloca sprites com mais de um estado
    al_destroy_bitmap(sprites->exit.closed);
    for (int i = 0; i < N_TRANSITION; ++i)
        al_destroy_bitmap(sprites->exit.open[i]);

    for (int i = 0; i < N_TRANSITION; ++i)
        al_destroy_bitmap(sprites->explosion.explosion[i]);

    al_destroy_bitmap(sprites->boulder.stop);
    for (int i = 0; i < N_TRANSITION; ++i)
        al_destroy_bitmap(sprites->boulder.roll[i]);

    al_destroy_bitmap(sprites->diamond.stop);
    for (int i = 0; i < N_TRANSITION; ++i)
    {
        al_destroy_bitmap(sprites->diamond.disappear[i]);
        al_destroy_bitmap(sprites->diamond.fall[i]);
    }

    al_destroy_bitmap(sprites->player.stop);
    for (int i = 0; i < N_TRANSITION; ++i)
    {
        al_destroy_bitmap(sprites->player.run_left[i]);
        al_destroy_bitmap(sprites->player.run_right[i]);
    }
}