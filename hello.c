#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define FRAMERATE 30.0

int main()
{
    float x = 0, y = 0;
    
    // Inicializa allegro
    if (!al_init())
    {
        perror("Não pôde inicializar o Allegro! :(\n");
        exit(1);
    }

    // Habilita leitura do teclado
    if (!al_install_keyboard())
    {
        perror("Não pôde inicializar o teclado :(\n");
        exit(1);
    }

    // Cria variável com timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FRAMERATE);
    if (!timer)
    {
        perror("Não pôde inicializar o temporizador :(\n");
        exit(1);
    }

    // Cria variável de fila de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue)
    {
        perror("Não pôde inicializar a fila :(\n");
        exit(1);
    }

    // Cria variável de display
    ALLEGRO_DISPLAY* disp = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!disp)
    {
        perror("Não pôde inicializar o display :(\n");
        exit(1);
    }

    // Cria variável com fonte built-in
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font)
    {
        perror("Não pôde inicializar a fonte :(\n");
        exit(1);
    }

    // Atribui eventos de teclado, display e timer à fila
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    // cria variáveis controle e de evento
    int redraw = 1, done = 0;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1)
    {
        // espera próximo evento
        al_wait_for_event(queue, &event);

        // escolhe o que acontece em cada caso
        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = 1;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        // caso done se torne 1, break
        if (done)
            break;

        // renderiza próximo quadro
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(130, 10, 150));
            al_draw_text(font, al_map_rgb(230, 100, 48), x+=2, y+=1, 0, "Hello world!");
            al_flip_display();

            redraw = 0;
        }
    }

    // desaloca todas as variáveis alocadas.
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
