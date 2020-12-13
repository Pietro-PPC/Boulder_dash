#ifndef __MAP__
#define __MAP__
 
#define BLANK     ' '
#define BORDER    '#'
#define BOULDER   'O'
#define DIAMOND   'D'
#define DIRT      '.'
#define EXIT      'E'
#define EXPLOSION 'X'
#define WALL      '|'
#define PLAYER    '@'

#define DIAMOND_WIN 2

#define LEVELFILE "level_1.txt"
#define MAP_TIMER 12

typedef struct tile_s
{
    int dx, dy;
    char type;
    int visited;
    int disappear;
} tile_t;

typedef struct map_s
{
    int width;       // largura do mapa
    int height;      // altura do mapa
    int player_x;    // coordenada x (horizontal) do jogador
    int player_y;    // coordenada y (vertical) do jogador
    int timer;       // temporizador para mudança de lugar das variáveis
    int cur_m;       // 0 para m[0] e 1 para m[1]
    int diamond_n;   // numero de diamantes do mapa
    int open_exit;   // saída aberta?
    tile_t **m[2];   // matriz de estado atual e próximo estado do mapa
} map_t;

void initialize_map(map_t *map);

void initialize_map_matrix(tile_t **m, int w, int h);

void initialize_tile(tile_t *t, char type);

void read_map(map_t *map);

void assign_tile(tile_t *d, tile_t *s);

int test_walkable(tile_t *t);

void destroy_map(map_t *map);

#endif