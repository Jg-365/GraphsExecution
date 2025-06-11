#ifndef LISTA_H
#define LISTA_H

// Estruturas da Lista
typedef struct NoLista {
    int valor;
    struct NoLista *prox;
} NoLista;

typedef struct Lista {
    NoLista *cabeca;
    int tamanho;
} Lista;

// Funções da Lista
void inicializarLista(Lista *l);
void inserirLista(Lista *l, int valor);
void liberarLista(Lista *l);

#endif
