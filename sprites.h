#include "map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#ifndef __SPRITES__
#define __SPRITES__

#define TILE_S 12
#define N_TRANSITION 6

// structs para entidades com mais de um estado visual
typedef struct player_bitmaps_s
{
    ALLEGRO_BITMAP *stop;
    ALLEGRO_BITMAP *run_left[N_TRANSITION];
    ALLEGRO_BITMAP *run_right[N_TRANSITION];
} player_bitmaps_t;

typedef struct diamond_bitmaps_s
{
    ALLEGRO_BITMAP *stop;
    ALLEGRO_BITMAP *fall[N_TRANSITION];
    ALLEGRO_BITMAP *disappear[N_TRANSITION];
} diamond_bitmaps_t;

typedef struct boulder_bitmaps_s
{
    ALLEGRO_BITMAP *stop;
    ALLEGRO_BITMAP *roll[N_TRANSITION];
} boulder_bitmaps_t;

typedef struct explosion_bitmaps_s
{
    ALLEGRO_BITMAP *explosion[N_TRANSITION];
} explosion_bitmaps_t;

typedef struct exit_bitmaps_s
{
    ALLEGRO_BITMAP *closed;
    ALLEGRO_BITMAP *open[N_TRANSITION];
} exit_bitmaps_t;


// struct para todos os bitmaps do jogo
typedef struct sprites_s
{
    ALLEGRO_BITMAP *sheet[2];
    ALLEGRO_BITMAP *hud;

    ALLEGRO_BITMAP *dirt;
    ALLEGRO_BITMAP *wall;
    ALLEGRO_BITMAP *blank;
    ALLEGRO_BITMAP *border;

    exit_bitmaps_t exit;
    explosion_bitmaps_t explosion;
    boulder_bitmaps_t boulder;
    diamond_bitmaps_t diamond;
    player_bitmaps_t player;

    ALLEGRO_BITMAP *background;
} sprites_t;

void init_sprites(sprites_t *sprites, map_t *map, ALLEGRO_DISPLAY *disp);

void initialize_sprites(sprites_t *sprites);

void destroy_sprites(sprites_t *sprites);

#endif