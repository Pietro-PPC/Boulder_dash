#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "audio.h"
#include "error_msg.h"
#include "game.h"

void initialize_audio(audio_t *audio)
// inicializa valores da struct de audio
{
    audio->diamond = NULL;
    audio->explosion = NULL;
    audio->boulder_hit = NULL;
    audio->step = NULL;
    audio->victory = NULL;
    audio->open_door = NULL;
    audio->wow = NULL;
}

ALLEGRO_SAMPLE *get_sample(char *filename)
// tenta inicializar sample de audio. Se conseguir,
// o retorna. Senão, jogo é abortado.
{
    ALLEGRO_SAMPLE *audio = al_load_sample(filename);
    test_init(audio, "algum dos audios");
    return audio;
}

void init_audio(audio_t *audio)
// carrega audios do jogo na struct de audio.
{
    audio->boulder_hit = get_sample("resources/sounds/boulder_hit.wav");
    audio->diamond = get_sample("resources/sounds/diamond.wav");
    audio->explosion = get_sample("resources/sounds/explosion.wav");
    audio->step = get_sample("resources/sounds/step.wav");
    audio->victory = get_sample("resources/sounds/victory.wav");
    audio->open_door = get_sample("resources/sounds/open_door.wav");
    audio->wow = get_sample("resources/sounds/wow.wav");
}

void play_sample(ALLEGRO_SAMPLE *sample, float v)
// Toca áudio passado como parâmetro
{
    al_play_sample(sample, v, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void play_audio(ALLEGRO_SAMPLE *sample, int *n_plays)
// Se áudio deve ser tocado, ele é tocado
{
    if (*n_plays)
    {
        play_sample(sample, 1.0);
        (*n_plays)--;
    }
}

void play_instant_samples(audio_t *audio, game_t *game)
// Toca áudios que são instantâneos à detecção de movimento.
{
    samples_t *n_plays = &(game->n_plays);

    play_audio(audio->boulder_hit, &(n_plays->boulder_hit));
    play_audio(audio->diamond, &(n_plays->diamond));
    play_audio(audio->victory, &(n_plays->victory));
    play_audio(audio->open_door, &(n_plays->open_door));
    play_audio(audio->wow, &(n_plays->wow));
}

void destroy_audio(audio_t *audio)
// Desaloca audios carregados
{
    al_destroy_sample(audio->boulder_hit);
    al_destroy_sample(audio->diamond);
    al_destroy_sample(audio->explosion);
    al_destroy_sample(audio->step);
    al_destroy_sample(audio->victory);
    al_destroy_sample(audio->open_door);
    al_destroy_sample(audio->wow);
}