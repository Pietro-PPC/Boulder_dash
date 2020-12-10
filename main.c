#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "states.h"

/* E AGORA? fazer transição entre tiles e atualizar posição. Arquivos: states.c, map.c */

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