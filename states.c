#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "states.h"
#include "map.h"
#include "error_msg.h"
#include "sprites.h"
#include "draw.h"

ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP *buffer;
ALLEGRO_FONT* font;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_EVENT event;

map_t map;
sprites_t sprites;

bool redraw = true;
bool done = false;

void state_initialize()
// Inicializa todas as variáveis e structs necessárias para o jogo
{
    // inicializa allegro
    test_init(al_init(), "Allegro");

    // inicializa teclado
    test_init(al_install_keyboard(), "teclado");

    // inicializa addon de imagens
    test_init(al_init_image_addon(), "addon de imagem");

    // inicializa timer de 1/FRAMERATE segundos
    timer = al_create_timer(FRAMERATE);
    test_init(timer, "temporizador");

    // inicializa display com altura e largura especificadas nas macros
    disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    test_init(disp, "tela de jogo");

    // inicializa buffer intermediario
    buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    test_init(buffer, "buffer intermediario");

    // inicializa fonte built-in
    font = al_create_builtin_font();
    test_init(font, "fonte");
    // inicializa fila de eventos
    queue = al_create_event_queue();
    test_init(queue, "fila de eventos");

    // registra teclado, display e timer como fontes de eventos da fila queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // inicializa variáveis de controle
    al_start_timer(timer);

    // le mapa do jogo
    initialize_map(&map);
    read_map(&map);

    // inicializa sprites
    initialize_sprites(&sprites);
    init_sprites(&sprites, &map, disp);

}
/*
void print_map(map_t *map)
{
    for (int i = 0; i < map->height; ++i)
    {
        for (int j = 0; j < map->width; ++j)
        {
            printf("%c", map->m[i][j].type);
        }
        printf("\n");
    }
}*/

void state_play()
{
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, ALLEGRO_KEY_MAX);
    
    // loop principal
    //print_map(&map);
    while(true)
    {
        al_wait_for_event(queue, &event);
        
        // age conforme evento ocorrido
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if (!(map.timer))
                {
                    update_map(&map, key);
                    for(int i = 0; i < ALLEGRO_KEY_MAX; ++i)
                        key[i] &= KEY_SEEN;
                }
                else
                    //transition(map->timer);
                    map.timer--;
                
                redraw = true;
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        if (key[ALLEGRO_KEY_ESCAPE])
            done = true;

        if (done)
            break;
        
        // desenhar na tela
        if (redraw)
        {
            pre_draw(buffer);
            draw_map(&sprites, &map);
            post_draw(buffer, disp);
            redraw = false;
        }
    }
}

void state_end()
{
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_bitmap(buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroy_sprites(&sprites);

    destroy_map(&map);
    
    al_shutdown_image_addon();

    exit(0);
}