#include <stdio.h>
#include "hi_score.h"
#include "error_msg.h"

void print_hi_scores(scores_t *scores)
{
    for (int i = 0; i < scores->size; ++i)
    {
        printf("%s %d\n", scores->name[i], scores->score[i]);
    }
}

void get_hi_scores(scores_t *scores)
{
    FILE *f;
    
    scores->size = 0;

    f = fopen (SCORE_FILE, "r");
    if (!f)
        fatal_error("Erro ao abrir arquivo de maiores pontuações");

    // itera para proxima linha
    while(getc(f) != '\n' && !feof(f));

    for (int i = 0; (i < SCORE_NUM) && !feof(f); ++i)
    {
        fscanf(f, "%4s", scores->name[i]);
        fscanf(f, "%d", scores->score + i);
        scores->size++;
    }

    fclose(f);
}