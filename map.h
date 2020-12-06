#ifndef __MAP__
#define __MAP__

#define BORDER '#'
#define BLANK  ' '

typedef struct map_s{
    int width;   // largura do mapa
    int height;  // altura do mapa
    char **m;    // matriz do mapa
} map_t;

void read_map(map_t *map);

void destroy_map(map_t *map);

//void print_map(map_t *map);

#endif