#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
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
ALLEGRO_EVENT event, menu_event;

scores_t scores;
game_t game;
audio_t audio;
sprites_t sprites;

unsigned char key[ALLEGRO_KEY_MAX];

void state_initialize()
// Inicializa todas as variáveis e structs necessárias para o jogo
{
    // inicializa allegro
    test_init(al_init(), "Allegro");

    // inicializa teclado
    test_init(al_install_keyboard(), "teclado");
    
    // inicializa addons
    test_init(al_init_image_addon(), "addon de imagem");
    test_init(al_init_primitives_addon(), "addon de primitivos");

    // funções de inicialização do audio
    test_init(al_install_audio(), "audio");
    test_init(al_init_acodec_addon(), "audio codecs");
    test_init(al_reserve_samples(16), "reserve samples");

    // inicializa variaveis de audio e carrega audios
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

    // le arquivo com maiores pontuações
    get_hi_scores(&scores);


    // inicializa vetor com teclas do teclado
    memset(key, 0, ALLEGRO_KEY_MAX);

    state = PLAY;
}

void state_play()
{
    bool redraw = true;
    bool done = false;
    state = FINISH;

    al_flush_event_queue(queue);
    // loop principal
    for(;;)
    {
        al_wait_for_event(queue, &event);
        
        // age conforme evento ocorrido
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // atualiza variáveis de tempo
                game.frame = (game.frame + 1) % 60;
                if (!game.frame)
                    --game.time;
                
                if (!(game.map.timer)) // se não há nenhuma entidade mudando de posição
                {
                    if (game.lives && !game.endgame) // se o jogo ainda não acabou
                    {
                        update_game(&game, key);
                        play_instant_samples(&audio, &game);
                    }
                    else // jogo acabou
                    {
                        if (!game.lives)
                        {
                            // se jogador perdeu, explode
                            end_game(&game);
                            game.n_plays.explosion = 1;
                            play_audio(audio.explosion, &(game.n_plays.explosion));
                        }
                        done = true;
                        state = ENDGAME;
                    }
                    // seta todas as teclas como vistas
                    for(int i = 0; i < ALLEGRO_KEY_MAX; ++i)
                        key[i] &= KEY_SEEN;
                }
                else // há alguma entidade mudando de posição
                {
                    // dois sons de passo são executados na transição de posição do player
                    if (game.n_plays.step)
                        if (game.map.timer % (MAP_TIMER/game.n_plays.step) == 0)
                            play_sample(audio.step, 0.3);
                    // decrementa timer de transição
                    game.map.timer--;
                }
                redraw = true;
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN: // tecla foi pressionada
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            
            case ALLEGRO_EVENT_KEY_UP: // tecla foi solta
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: // display fechado
                done = true;
                break;
        }
        if (key[ALLEGRO_KEY_ESCAPE] || !game.time) // jogador desistiu ou acabou o tempo
            game.lives = 0;

        else if (key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1])
        // jogador abriu menu de instruções
        {
            key[ALLEGRO_KEY_H] = 0;
            key[ALLEGRO_KEY_F1] = 0;
            state = INSTRUCTIONS;
            done = true;
        }

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

void state_instructions()
// estado do menu de instruções
{
    state = PLAY;

    int instruction_num = 0;
    int done = 0;
    int draw = 1;
    
    // limpa fila de eventos (teclas F1 e/ou H apertadas)
    al_flush_event_queue(queue);
    for (;;)
    {
        al_wait_for_event(queue, &menu_event);

        if (menu_event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            switch (menu_event.keyboard.keycode)
            {
                case ALLEGRO_KEY_RIGHT: // vai para próxima página de menu
                    instruction_num = (instruction_num+1)%3;
                    draw = 1;
                    break;
                case ALLEGRO_KEY_LEFT: // volta uma página de menu
                    if (instruction_num)
                        instruction_num--;
                    else
                        instruction_num = 2;
                    draw = 1;
                    break;
                case ALLEGRO_KEY_F1: // sai do menu
                case ALLEGRO_KEY_H:
                    done = 1;
                    break;
            }
        }

        if (done)
            break;

        if (draw) // desenha men de instruções
        {
            pre_draw(buffer);
            draw_game(&sprites, &(game), font);
            draw_instructions(&sprites, font, instruction_num+1);
            post_draw(buffer, disp);
            draw = 0;
        }

    }
}

void state_endgame()
// termina o jogo e mostra maiores pontuações
{
    state = FINISH;
    bool done = false;

    // mostra últimas animações do jogo
    while(game.map.timer > 0)
    {
        pre_draw(buffer);
        draw_game(&sprites, &(game), font);
        post_draw(buffer, disp);
        game.map.timer--;
    }

    // insere pontuação no vetor de pontuações
    insert_score(&scores, "???", game.score);

    // imprime maiores pontuações na frente do jogo
    pre_draw(buffer);
    draw_game(&sprites, &(game), font);
    draw_hi_scores(&scores, font);
    post_draw(buffer, disp);
    
    // salva maiores pontuações no arquivo
    save_hi_scores(&scores);

    // aguarda tecla esc ou fechar o display
    while (1)
    {
        al_wait_for_event(queue, &event);

        if ((event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE ) ||
            event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;
        
        if (done)
            break;
    }
}

void state_finish()
// desaloca todas as estruturas usadas no jogo
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
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();

    exit(0);
}