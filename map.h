#ifndef __MAP__
#define __MAP__

#define BORDER  '#'
#define BLANK   ' '
#define DIRT    '.'
#define BOULDER 'O'
#define WALL    '|'
#define DIAMOND 'D'
#define PLAYER  '@'

#define LEVELFILE "level_1.txt"

typedef struct map_s{
    int width;   // largura do mapa
    int height;  // altura do mapa
    char **m;    // matriz do mapa
} map_t;

void read_map(map_t *map);

void destroy_map(map_t *map);

#endif