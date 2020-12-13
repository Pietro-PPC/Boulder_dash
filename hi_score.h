#ifndef __HISCORE__
#define __HISCORE__

#define SCORE_FILE "data/hi_scores.txt"
#define SCORE_NUM 5

typedef struct scores_s
{
    char name[SCORE_NUM][4];
    int score[SCORE_NUM];
    int size;
} scores_t;

void print_hi_scores(scores_t *scores);

void get_hi_scores(scores_t *scores);

#endif