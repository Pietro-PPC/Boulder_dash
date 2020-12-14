#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "states.h"

int main()
{
    state = INITIALIZE;

    for (;;)
    {
        switch(state)
        {
            case INITIALIZE: state_initialize(); break;
            case PLAY: state_play(); break;
            case INSTRUCTIONS: state_instructions(); break;
            case ENDGAME: state_endgame(); break;
            case FINISH: state_finish(); break;
        }
    }
}