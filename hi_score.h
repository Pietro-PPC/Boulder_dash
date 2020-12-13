#ifndef __HISCORE__
#define __HISCORE__

#define SCORE_FILE "data/hi_scores.txt"
#define SCORE_NUM 5

typedef struct scores_s
{
    char name[SCORE_NUM+1][4];
    int score[SCORE_NUM+1];
    int size;
} scores_t;

void save_hi_scores(scores_t *scores);

void get_hi_scores(scores_t *scores);

void insert_score(scores_t *scores, char *name, int score);

#endif