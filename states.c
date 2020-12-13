#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "states.h"
#include "map.h"
#include "error_msg.h"
#include "sprites.h"
#include "draw.h"
#include "update.h"
#include "game.h"
#include "audio.h"
#include "hi_score.h"

ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP *buffer;
ALLEGRO_FONT* font;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_EVENT event;

scores_t scores;
game_t game;
audio_t audio;
sprites_t sprites;

void state_initialize()
// Inicializa todas as variáveis e structs necessárias para o jogo
{
    // inicializa allegro
    test_init(al_init(), "Allegro");

    // inicializa teclado
    test_init(al_install_keyboard(), "teclado");

    // inicializa addon de imagens
    test_init(al_init_image_addon(), "addon de imagem");

    // funções de inicialização do audio
    test_init(al_install_audio(), "audio");
    test_init(al_init_acodec_addon(), "audio codecs");
    test_init(al_reserve_samples(16), "reserve samples");

    initialize_audio(&audio);
    init_audio(&audio);

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

    // inicializa jogo
    initialize_game(&game);

    // le mapa do jogo
    read_map(&(game.map));

    // inicializa sprites
    initialize_sprites(&sprites);
    init_sprites(&sprites, &(game.map), disp);

    state = PLAY;

    get_hi_scores(&scores);
    print_hi_scores(&scores);
    printf("\n");
    insert_score(&scores, "PIE", 50);
    print_hi_scores(&scores);
}

void state_play()
{
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, ALLEGRO_KEY_MAX);
    bool redraw = true;
    bool done = false;

    state = FINISH;

    // loop principal
    while(true)
    {
        al_wait_for_event(queue, &event);
        
        // age conforme evento ocorrido
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                game.frame = (game.frame + 1) % 60;
                if (!game.frame)
                    --game.time;
                
                if (!(game.map.timer))
                {
                    if (game.lives)
                    {
                        update_game(&game, key);
                        play_instant_samples(&audio, &game);
                    }
                    else
                    {
                        end_game(&game);
                        game.n_plays.explosion = 1;
                        play_audio(audio.explosion, &(game.n_plays.explosion));
                        done = true;
                        state = ENDGAME;
                    }
                    for(int i = 0; i < ALLEGRO_KEY_MAX; ++i)
                        key[i] &= KEY_SEEN;
                }
                else
                {
                    if(game.n_plays.step)
                        if (game.map.timer % (MAP_TIMER/game.n_plays.step) == 0)
                            play_sample(audio.step, 0.3);
                    game.map.timer--;
                }
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
            draw_game(&sprites, &(game), font);
            post_draw(buffer, disp);
            redraw = false;
        }
    }
}

void state_endgame()
{
    state = FINISH;
    bool done = false;

    while(game.map.timer > 0)
    {
        pre_draw(buffer);
        draw_game(&sprites, &(game), font);
        post_draw(buffer, disp);
        game.map.timer--;
    }

    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;
        
        if (done)
            break;
    }
}

void state_finish()
{
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_bitmap(buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroy_sprites(&sprites);
    destroy_map(&(game.map));
    destroy_audio(&audio);
    
    al_shutdown_image_addon();

    exit(0);
}