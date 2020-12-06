#ifndef __STATES__
#define __STATES__

#define FRAMERATE 1.0/60.0

#define BUFFER_WIDTH  360
#define BUFFER_HEIGHT 240

#define SCALE 3
#define DISPLAY_WIDTH  BUFFER_WIDTH * SCALE 
#define DISPLAY_HEIGHT BUFFER_HEIGHT * SCALE

void state_initialize();

void state_play();

void state_end();

#endif