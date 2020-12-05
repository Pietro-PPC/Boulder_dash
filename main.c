#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "states.h"

enum {INITIALIZE, PLAY, END} state;

int main()
{
    state = INITIALIZE;

    for (;;)
    {
        switch(state)
        {
            case INITIALIZE: state_initialize(); state = PLAY; break;
            case PLAY: state_play(); state = END; break;
            case END: state_end(); break;
        }
    }
}