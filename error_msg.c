#include <stdio.h>
#include <allegro5/allegro.h>
#include "error_msg.h"

void fatal_error(char *message)
// Imprime message em stderr e aborta
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void test_init(bool test, char *message)
// Testa retorno de função de inicialização do Allegro
{
    if (test) return;
    
    fprintf(stderr, "Erro ao inicializar %s :(\n", message);
    exit(1);
}