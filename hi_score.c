#include <stdio.h>
#include "hi_score.h"
#include "error_msg.h"
#include "string.h"

void save_hi_scores(scores_t *scores)
{
    FILE *f;

    f = fopen(SCORE_FILE, "w");
    if (!f)
        fatal_error("erro ao imprimir maiores pontuações");

    fprintf(f, "%s %d", scores->name[0], scores->score[0]);
    for (int i = 1; i < scores->size; ++i)
        fprintf(f, "\n%s %d", scores->name[i], scores->score[i]);
    
    fclose(f);
}

void insert_score(scores_t *scores, char *name, int score)
{
    int i = scores->size;
    while(score > scores->score[i-1] && i > 0)
    {
        i--;
        scores->score[i+1] = scores->score[i];
    }
    int pos = i;
    scores->score[pos] = score;

    for (i = scores->size; i > pos; --i)
        strcpy(scores->name[i], scores->name[i-1]);
    strcpy(scores->name[pos], name);

    if (scores->size < SCORE_NUM)
        ++scores->size;
}

void get_hi_scores(scores_t *scores)
{
    FILE *f;
    
    scores->size = 0;

    f = fopen (SCORE_FILE, "r");
    if (f)
    {
        for (int i = 0; (i < SCORE_NUM) && !feof(f); ++i)
        {
            fscanf(f, "%4s", scores->name[i]);
            fscanf(f, "%d", scores->score + i);
            scores->size++;
        }
        fclose(f);
    }

}