#ifndef __STATES__
#define __STATES__

#define FRAMERATE 1.0/60.0

enum {INITIALIZE, PLAY, ENDGAME, FINISH} state;

#define BUFFER_WIDTH  480
#define BUFFER_HEIGHT 288

#define SCALE 3
#define DISPLAY_WIDTH  BUFFER_WIDTH * SCALE 
#define DISPLAY_HEIGHT BUFFER_HEIGHT * SCALE

#define KEY_SEEN 1
#define KEY_RELEASED 2

void state_initialize();

void state_play();

void state_endgame();

void state_finish();

#endif