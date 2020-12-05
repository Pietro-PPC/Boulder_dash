#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#define FRAMERATE 60.0

#define DISPLAY_WIDTH  1280 
#define DISPLAY_HEIGHT 800

void test_init(bool test, char *message)
// Testa retorno de função de inicialização do Allegro
{
    if (!test)
    {
        fprintf(stderr, "Erro ao inicializar %s :(\n", message);
        exit(1);
    }
}

int main()
{
    // inicializa allegro
    test_init(al_init(), "Allegro");

    // inicializa teclado
    test_init(al_install_keyboard(), "teclado");

    // inicializa timer de 1/FRAMERATE segundos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FRAMERATE);
    test_init(timer, "temporizador");

    // inicializa display 320x200
    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    test_init(disp, "tela de jogo");

    // inicializa fonte built-in
    ALLEGRO_FONT* font = al_create_builtin_font();
    test_init(font, "fonte");
    // inicializa fila de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    test_init(queue, "fila de eventos");

    // registra teclado, display e timer como fontes de eventos da fila queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // inicializa variáveis de controle
    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(true)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            case ALLEGRO_EVENT_KEY_DOWN:
                done = true;
                break;
        }

        if (done)
            break;
        
        // desenhar na tela
        if (redraw)
        {
            al_clear_to_color(al_map_rgb(106, 13, 176));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello World!");
            al_flip_display();
        }

    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}