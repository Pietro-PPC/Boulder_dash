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
#define MAP_TIMER 12

typedef struct tile_s
{
    int dx, dy;
    char type;
    int visited;
} tile_t;

typedef struct map_s
{
    int width;     // largura do mapa
    int height;    // altura do mapa
    int player_x;  // coordenada x (vertical) do jogador
    int player_y;  // coordenada y (horizontal) do jogador
    int timer;     // temporizador para mudança de lugar das variáveis
    int cur_m;     // 0 para m[0] e 1 para m[1]
    tile_t **m[2]; // matriz de estado atual e estado anterior
} map_t;

void initialize_map(map_t *map);

void read_map(map_t *map);

void update_map(map_t *map, unsigned char *key);

void destroy_map(map_t *map);

#endif