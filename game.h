#include "map.h"

#ifndef __GAME__
#define __GAME__

// struct que determina se cada sample de áudio será tocado ou não nessa iteração
typedef struct samples_s
{
    int diamond;
    int explosion;
    int boulder_hit;
    int step;
    int victory;
    int open_door;
    int wow;
} samples_t;

// struct com variáveis correspondentes ao estado do jogo
typedef struct game_s
{
    map_t map;         // mapa do jogo
    samples_t n_plays; // struct com vezes que os audios serão tocados
    int lives;         // vidas do jogador
    int score;         // pontuação do jogador
    int time;          // tempo restante na partida
    int frame;         // quadro em que o jogo se encontra (mod 60)
    int diamonds_got;  // diamantes que foram pegos pelo jogador
    int endgame;       // será diferente de 0 se o jogo acabou
} game_t;

void initialize_samples(samples_t *s);

void initialize_game(game_t *game);

#endif