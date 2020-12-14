#include <stdio.h>
#include "hi_score.h"
#include "error_msg.h"
#include "string.h"

void save_hi_scores(scores_t *scores)
// Imprime vetor com maiores pontuações e seus jogadores em um arquivo
// Se não há arquivo com maiores pontuações, um é criado
{
    FILE *f;

    f = fopen(SCORE_FILE, "w");
    if (!f)
        fatal_error("erro ao imprimir maiores pontuações");

    // escreve primeiro lugar
    fprintf(f, "%s %d", scores->name[0], scores->score[0]);
    // escreve próximos lugares
    for (int i = 1; i < scores->size; ++i)
        fprintf(f, "\n%s %d", scores->name[i], scores->score[i]);
    
    fclose(f);
}

void insert_score(scores_t *scores, char *name, int score)
// Insere pontuação ordenada no vetor de maiores pontuações.
// Se pontuação não é alta o suficiente, fica em posição de sentinela
// e não será mostrada.
{
    // desloca scores menores para depois do score atual
    int i = scores->size;
    while(score > scores->score[i-1] && i > 0)
    {
        i--;
        scores->score[i+1] = scores->score[i];
    }
    int pos = i;
    // insere score atual na sua posição devida
    scores->score[pos] = score;

    // insere nome correspondente ao score na posição certa do vetor de nomes
    for (i = scores->size; i > pos; --i)
        strcpy(scores->name[i], scores->name[i-1]);
    strcpy(scores->name[pos], name);

    // se o vetor ainda não chegou ao seu limite de tamanho, ele é incrementado
    if (scores->size < SCORE_NUM)
        ++scores->size;
}

void get_hi_scores(scores_t *scores)
// le arquivo com maiores pontuações, se houver. Pontuações armazenadas
// em um vetor.
{
    FILE *f;
    
    scores->size = 0;

    f = fopen (SCORE_FILE, "r");
    // se o arquivo existe, pontuações são lidas
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