#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "map.h"
#include "sprites.h"
#include "draw.h"
#include "states.h"
#include "game.h"

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

void draw_tile(tile_t *t, game_t *game, int x, int y, sprites_t *sprites)
{
    map_t *map = &(game->map);
    int timer = MAP_TIMER - map->timer;
    int state = timer/2;
    if (timer == 12) state = 5;
    
    int exit_state;

    switch (t->type)
    {
        case BORDER: 
            al_draw_bitmap(sprites->border, x, y, 0);
            break;
        case DIRT:
            if (!t->disappear)
                al_draw_bitmap(sprites->dirt, x, y, 0);
            break;
        case BOULDER:
            if (t->dx < 0)
                al_draw_bitmap(sprites->boulder.roll[state], x, y, 0);
            else if (t->dx > 0)
                al_draw_bitmap(sprites->boulder.roll[N_TRANSITION - (state+1)], x, y, 0);
            else
                al_draw_bitmap(sprites->boulder.stop, x, y, 0);
            break;
        case WALL:
            al_draw_bitmap(sprites->wall, x, y, 0);
            break;
        case DIAMOND:
            if (t->disappear)
                al_draw_bitmap(sprites->diamond.disappear[state], x, y, 0);
            else if (t->dy || t->dx)
                al_draw_bitmap(sprites->diamond.fall[state], x, y, 0);
            else
                al_draw_bitmap(sprites->diamond.stop, x, y, 0);
            break;
        case PLAYER:
            if (t->dx > 0 || t->dy)
                al_draw_bitmap(sprites->player.run_right[state], x, y, 0);
            else if (t->dx < 0)
                al_draw_bitmap(sprites->player.run_left[state], x, y, 0);
            else
                al_draw_bitmap(sprites->player.stop, x, y, 0);
            break;
        case EXPLOSION:
            al_draw_bitmap(sprites->explosion.explosion[state], x, y, 0);
            break;
        case EXIT:
            exit_state = (game->frame/6)%6;
            if (map->open_exit)
                al_draw_bitmap(sprites->exit.open[exit_state], x, y, 0);
            else
                al_draw_bitmap(sprites->exit.closed, x, y, 0);
            break;
        case FAKE_WALL:
            if (!t->disappear)
                al_draw_bitmap(sprites->wall, x, y, 0);
            break;
    }
}

void draw_map(sprites_t *sprites, game_t *game)
{
    map_t *map = &(game->map);
    al_draw_bitmap(sprites->background, 0, 0, 0);

    tile_t **mat = map->m[map->cur_m];
    int x, y;
    int timer = MAP_TIMER - map->timer;

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
            if (!mat[i][j].dx && !mat[i][j].dy)
            {
                x = TILE_S*(j-1) + timer * mat[i][j].dx;
                y = TILE_S*(i-1) + timer * mat[i][j].dy;
                draw_tile(&(mat[i][j]), game, x, y, sprites);
            }

    for (int i = 1; i <= map->height; ++i)
        for (int j = 1; j <= map->width; ++j)
            if (mat[i][j].dx || mat[i][j].dy)
            {
                x = TILE_S*(j-1) + timer * mat[i][j].dx;
                y = TILE_S*(i-1) + timer * mat[i][j].dy;
                draw_tile(&(mat[i][j]), game, x, y, sprites);
            }
}

void draw_hud(sprites_t *sprites, game_t *game, ALLEGRO_FONT *font)
{
    char time_text[5];
    char diamond_text[10];
    char score_text[6];

    snprintf(score_text, 6, "%05d", game->score);
    snprintf(time_text, 5, "%d", game->time);
    snprintf(diamond_text, 10, "%d/%d", game->diamonds_got, DIAMOND_WIN);
    al_draw_bitmap(sprites->hud, 0, 264, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 140, 272, 0, diamond_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), 235, 272, 0, time_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), 309, 272, 0, score_text);
}

void draw_game(sprites_t *sprites, game_t *game, ALLEGRO_FONT *font)
{
    draw_map(sprites, game);
    draw_hud(sprites, game, font);
}