#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "audio.h"
#include "error_msg.h"
#include "game.h"

void initialize_audio(audio_t *audio)
{
    audio->diamond = NULL;
    audio->explosion = NULL;
    audio->boulder_hit = NULL;
    audio->step = NULL;
    audio->victory = NULL;
}

ALLEGRO_SAMPLE *get_sample(char *filename)
{
    ALLEGRO_SAMPLE *audio = al_load_sample(filename);
    test_init(audio, "algum dos audios");
    return audio;
}

void init_audio(audio_t *audio)
{
    audio->boulder_hit = get_sample("sounds/boulder_hit.wav");
    audio->diamond = al_load_sample("sounds/diamond.wav");
    audio->explosion = al_load_sample("sounds/explosion.wav");
    audio->step = al_load_sample("sounds/step.wav");
    audio->victory = al_load_sample("sounds/victory.wav");
}

void play_audio(ALLEGRO_SAMPLE *sample)
{
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void play_instant_samples(audio_t *audio, game_t *game)
{
    //int timer = game->map.timer;
    samples_t *n_plays = &(game->n_plays);

    if (n_plays->boulder_hit)
    {
        play_audio(audio->boulder_hit);
        n_plays->boulder_hit = 0;
    }

    if (n_plays->diamond)
    {
        play_audio(audio->diamond);
        n_plays->diamond = 0;
    }

    //if ()
}

void destroy_audio(audio_t *audio)
{
    al_destroy_sample(audio->boulder_hit);
    al_destroy_sample(audio->diamond);
    al_destroy_sample(audio->explosion);
    al_destroy_sample(audio->step);
    al_destroy_sample(audio->victory);
}