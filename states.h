#ifndef __STATES__
#define __STATES__

// enum global com estados do jogo
enum {INITIALIZE, PLAY, INSTRUCTIONS, ENDGAME, FINISH} state;

// define largura e altura do buffer intermediário do jogo
#define BUFFER_WIDTH  480
#define BUFFER_HEIGHT 288

// define escala da janela do jogo em relação ao buffer intermediário
#define SCALE 3
#define DISPLAY_WIDTH  BUFFER_WIDTH * SCALE 
#define DISPLAY_HEIGHT BUFFER_HEIGHT * SCALE

// macros para controle do teclado
#define KEY_SEEN 1
#define KEY_RELEASED 2

void state_initialize();

void state_play();

void state_endgame();

void state_instructions();

void state_finish();

#endif