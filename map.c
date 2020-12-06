/* ideia: detectar e avisar erro no mapa */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

void initialize_map(map_t *map)
{
    map->width = 0;
    map->height = 0;
    map->m = NULL;
}

void fatal_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
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

void print_map(map_t *map)
{
    for (int i = 0; i < map->width * map->height; ++i)
    {
        printf("%c", map->m[0][i]);
        if (!((i+1) % map->width))
            printf("\n");
    }
}

void read_map(map_t *map)
{
    FILE *f;

    f = fopen("levelmaps/level_1.txt", "r");
    if (!f)
        fatal_error("Falha ao abrir mapa");

    // lê linha com largura e altura e pula para a próxima
    fscanf(f, "%d %d", &(map->width), &(map->height));
    while(getc(f) != '\n' && !feof(f));
    allocate_map(map);

    // Inicializa mapa com valores em branco
    memset(map->m[0], BLANK, map->width * map->height + 1);

    int i;
    for (i = 0; i < map->height && !feof(f); ++i)
    {
        fgets(map->m[i], map->width + 1, f);

        // itera para proxima linha ou fim de arquivo
        while(getc(f) != '\n' && !feof(f));
    }
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