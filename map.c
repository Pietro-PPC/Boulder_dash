/* ideia: detectar e avisar erro no mapa */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_msg.h"
#include "map.h"
#include "sprites.h"
#include "entities.h"

void initialize_map(map_t *map)
{
    map->width = 0;
    map->height = 0;
    map->m = NULL;
}

void allocate_map(map_t *map)
{
    if (!map->width || !map->height)
        fatal_error("Mapa sem dimensões");

    map->m = malloc(map->height * sizeof(char *));
    if (!map)
        fatal_error("Falha ao alocar mapa");

    // +1 para armazenar o \0 ao final
    map->m[0] = malloc(map->width * map->height * sizeof(char) + 1); 
    if (!map->m[0])
        fatal_error("Falha ao alocar mapa");

    for (int i = 1; i < map->height; ++i)
        map->m[i] = map->m[0] + i * map->width;
}

void read_map(map_t *map, player_t *p)
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
    memset(map->m[0], BLANK, map->width * map->height + 1);

    int i, aux;
    for (i = 0; i < map->height && !feof(f); ++i)
    {
        fgets(map->m[i], map->width + 1, f);
        aux = strcspn(map->m[i], "@");
        if (aux < strlen(map->m[i]))
        {
            p->x = aux;
            p->y = i;
        }

        // itera para proxima linha ou fim de arquivo
        while(getc(f) != '\n' && !feof(f));
    }
}

#define P_TIMER 6

void update_map(map_t *map, player_t *p, unsigned char *key)
{
    if (!p->timer)
    {
        if (key[ALLEGRO_KEY_LEFT])
        {
            map->m[p->y][p->x - 1] = PLAYER;
            map->m[p->y][p->x] = BLANK;
            (p->x)--;
            p->timer = P_TIMER;
        }
        else if (key[ALLEGRO_KEY_RIGHT])
        {
            map->m[p->y][p->x + 1] = PLAYER;
            map->m[p->y][p->x] = BLANK;
            (p->x)++;
            p->timer = P_TIMER;
        }
        else if (key[ALLEGRO_KEY_UP])
        {
            map->m[p->y - 1][p->x] = PLAYER;
            map->m[p->y][p->x] = BLANK;
            (p->y)--;
            p->timer = P_TIMER;
        }
        else if (key[ALLEGRO_KEY_DOWN])
        {
            map->m[p->y + 1][p->x] = PLAYER;
            map->m[p->y][p->x] = BLANK;
            (p->y)++;
            p->timer = P_TIMER;
        }

        for(int i = 0; i < ALLEGRO_KEY_MAX; ++i)
            key[i] &= KEY_SEEN;
    }
    else
        p->timer--;
        
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