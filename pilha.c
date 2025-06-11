#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = NULL;
    p->tamanho = 0;
}

// Empilha um valor
void push(Pilha *p, int valor) {
    NoPilha *novoNo = (NoPilha *)malloc(sizeof(NoPilha));
    if (!novoNo) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->valor = valor;
    novoNo->baixo = p->topo;
    p->topo = novoNo;
    p->tamanho++;
}

// Desempilha e retorna o valor
int pop(Pilha *p) {
    if (p->topo == NULL) {
        fprintf(stderr, "Pilha vazia.\n");
        exit(EXIT_FAILURE);
    }
    NoPilha *remover = p->topo;
    int valor = remover->valor;
    p->topo = remover->baixo;
    free(remover);
    p->tamanho--;
    return valor;
}

// Retorna o valor no topo sem remover
int topo(Pilha *p) {
    if (p->topo == NULL) {
        fprintf(stderr, "Pilha vazia.\n");
        exit(EXIT_FAILURE);
    }
    return p->topo->valor;
}

// Libera toda a pilha
void liberarPilha(Pilha *p) {
    while (p->topo != NULL) {
        pop(p);
    }
}

