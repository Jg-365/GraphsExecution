#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Inicializa a lista
void inicializarLista(Lista *l) {
    l->cabeca = NULL;
    l->tamanho = 0;
}

// Insere um valor no início
void inserirLista(Lista *l, int valor) {
    NoLista *novoNo = (NoLista *)malloc(sizeof(NoLista));
    if (!novoNo) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->valor = valor;
    novoNo->prox = l->cabeca;
    l->cabeca = novoNo;
    l->tamanho++;
}

// Libera toda a lista
void liberarLista(Lista *l) {
    NoLista *atual = l->cabeca;
    while (atual != NULL) {
        NoLista *remover = atual;
        atual = atual->prox;
        free(remover);
    }
    l->cabeca = NULL;
    l->tamanho = 0;
}

