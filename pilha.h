#ifndef PILHA_H
#define PILHA_H

// Estruturas da Pilha
typedef struct NoPilha {
    int valor;
    struct NoPilha *baixo;
} NoPilha;

typedef struct Pilha {
    NoPilha *topo;
    int tamanho;
} Pilha;

// Fun��es da Pilha
void inicializarPilha(Pilha *p);
void push(Pilha *p, int valor);
int pop(Pilha *p);
int topo(Pilha *p);
void liberarPilha(Pilha *p);

#endif

