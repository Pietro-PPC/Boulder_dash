#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "map.h"
#include "sprites.h"
#include "draw.h"
#include "states.h"
#include "game.h"
#include "hi_score.h"

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
    al_draw_filled_rectangle(375, 267, 469, 283, al_map_rgba_f(0, 0, 0, 0.7));
    al_draw_text(font, al_map_rgb(255, 255, 255), 383, 272, 0, "HELP: h/F1");
}

void draw_hi_scores(scores_t *scores, ALLEGRO_FONT *font)
{
    int height = BUFFER_HEIGHT - 4*TILE_S;
    int width = BUFFER_WIDTH - 2*TILE_S; 
    al_draw_filled_rectangle(2*TILE_S, 2*TILE_S, width, height, al_map_rgba_f(0, 0, 0, 0.9));
    al_draw_text(font, al_map_rgb(255, 255, 255), 170, 3*TILE_S, 0, "H I - S C O R E S");

    for (int i = 0; i < scores->size; ++i)
    {
        char pts[10];
        sprintf(pts, "%d", scores->score[i]);
        al_draw_text(font, al_map_rgb(255, 255, 255), 170, (7 + 2*i)*TILE_S, 0, scores->name[i]);
        al_draw_text(font, al_map_rgb(255, 255, 255), 270, (7 + 2*i)*TILE_S, 0, pts);
    }

    al_draw_text(font, al_map_rgb(255, 255, 255), 140, 19*TILE_S, 0, "PRESSIONE ESC PARA SAIR...");
}

void draw_instructions(sprites_t *sprites, ALLEGRO_FONT *font, int n)
{
    int height = BUFFER_HEIGHT - 4*TILE_S;
    int width = BUFFER_WIDTH - 2*TILE_S;
    al_draw_filled_rectangle(2*TILE_S, 2*TILE_S, width, height, al_map_rgba_f(0, 0, 0, 0.9));

    char page[4];
    sprintf(page, "%d/3", n);
    al_draw_text(font, al_map_rgb(255, 255, 255), 160, 3*TILE_S, 0, "I N S T R U Ç Õ E S");
    al_draw_text(font, al_map_rgb(255, 255, 255), 420, 3*TILE_S, 0, page);

    if (n == 1)
    {
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 7*TILE_S, 0,
                        "O objetivo do Boulder Dash é bem simples. Colete");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 8*TILE_S, 0,
                        "certo número de diamantes para abrir a saída do");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 9*TILE_S, 0,
                        "mapa e mova-se até ela para terminar o nível. Sua");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 10*TILE_S, 0,
                        "pontuação será maior se você coletar mais diamantes");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 11*TILE_S, 0,
                        "e chegar à saída do nível em um tempo menor.");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 13*TILE_S, 0,
                        "Para se mover pelo mapa, (e por estas instruções),");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 14*TILE_S, 0,
                        "use as setas do seu teclado. Para desistir da");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 15*TILE_S, 0,
                        "partida atual, pressione a tecla ESC.");
    }
    else if (n == 2)
    {
        al_draw_scaled_bitmap(sprites->player.stop, 
                            0, 0, TILE_S, TILE_S, 
                            3*TILE_S, 5*TILE_S, 1.5*TILE_S, 1.5*TILE_S, 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 4.5*TILE_S, 0,
                        "Jogador: este é você, o herói do jogo! Colete o");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 5.5*TILE_S, 0,
                        "máximo possível de diamantes e encontre a saída.");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 6.5*TILE_S, 0,
                        "no menor tempo possível.");

        al_draw_scaled_bitmap(sprites->boulder.stop, 
                            0, 0, TILE_S, TILE_S, 
                            3*TILE_S, 8.5*TILE_S, 1.5*TILE_S, 1.5*TILE_S, 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 8.5*TILE_S, 0,
                        "Pedra: pode ser empurrada, mas não deixe-a");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 9.5*TILE_S, 0,
                        "cair na sua cabeça!");

        al_draw_scaled_bitmap(sprites->diamond.stop, 
                            0, 0, TILE_S, TILE_S, 
                            3*TILE_S, 11.5*TILE_S, 1.5*TILE_S, 1.5*TILE_S, 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 11.5*TILE_S, 0,
                        "Diamante: colete o suficiente e poderá sair da");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 12.5*TILE_S, 0,
                        "fase. Mas tome cuidado com sua cabeça também!");

        al_draw_scaled_bitmap(sprites->exit.closed, 
                            0, 0, TILE_S, TILE_S, 
                            3*TILE_S, 14.5*TILE_S, 1.5*TILE_S, 1.5*TILE_S, 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 14.5*TILE_S, 0,
                        "Saída: colete diamantes suficientes e a porta");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 15.5*TILE_S, 0,
                        "de saída se abrirá.");

        al_draw_scaled_bitmap(sprites->wall, 
                            0, 0, TILE_S, TILE_S, 
                            3*TILE_S, 17.5*TILE_S, 1.5*TILE_S, 1.5*TILE_S, 0);
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 17.5*TILE_S, 0,
                        "Paredes: Não há muito o que falar sobre elas.");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5*TILE_S, 18.5*TILE_S, 0,
                        "Apenas bloqueiam seu caminho.");
    }
    else if (n == 3)
    {
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 5*TILE_S, 0,
                        "Jogo desenvolvido por Pietro Polinari Cavassin,");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 6*TILE_S, 0,
                        "aluno do curso de bacharelado em ciência da");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 7*TILE_S, 0,
                        "computação pela UFPR, para a disciplina de");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 8*TILE_S, 0,
                        "programação 2, ministrada pelo professor Carlos");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 9*TILE_S, 0,
                        "Maziero.");

        al_draw_text(font, al_map_rgb(255, 255, 255), 15*TILE_S, 11*TILE_S, 0,
                        "C R É D I T O S");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 12.5*TILE_S, 0,
                        "Design dos spritesheets:            Retrocade.net");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 14*TILE_S, 0,
                        "Efeitos sonoros:                            JSFXR");
        al_draw_text(font, al_map_rgb(255, 255, 255), 3*TILE_S, 15.5*TILE_S, 0,
                        "Biblioteca de desenvolvimento:          Allegro 5");

    }
}

void draw_game(sprites_t *sprites, game_t *game, ALLEGRO_FONT *font)
{
    draw_map(sprites, game);
    draw_hud(sprites, game, font);
}