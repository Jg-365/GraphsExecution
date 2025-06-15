#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "grafo.h"
#include "lista.h"
#include "pilha.h"
#include <math.h>
#include <time.h>

void gerarRedeSocial(Usuario* usuarios, int n) {

    for (int i = 0; i < n; i++) {
        int num_seguidos = 1 + rand() % ((int)log2(n) + 1);
        for (int k = 0; k < num_seguidos; k++) {
            int j;
            do {
                j = rand() % n;
            } while (j == i);
            adicionarSeguido(usuarios, i, j);
        }
    }

    // Garantir que cada usuário tenha pelo menos 1 seguidor
    for (int i = 0; i < n; i++) {
        if (usuarios[i].seguidoPor.cabeca == NULL) {
            int j;
            do {
                j = rand() % n;
            } while (j == i);
            adicionarSeguido(usuarios, j, i);  
        }
    }
}

void exportarParaGrafoListaReverso(Usuario *u, int n, GrafoLista *gt) {
    for (int i = 0; i < n; ++i)
        for (NoLista *p = u[i].segue.cabeca; p; p = p->prox)
            adicionarArestaLista(gt, p->valor, i);   /* inverte */
}

void exportarParaGrafoMatrizReverso(Usuario *u, int n, GrafoMatriz *gt) {
    for (int i = 0; i < n; ++i)
        for (NoLista *p = u[i].segue.cabeca; p; p = p->prox)
            adicionarArestaMatriz(gt, p->valor, i); /* inverte */
}

void exportarParaGrafoLista(Usuario *u, int n, GrafoLista *g)
{
    for (int i = 0; i < n; ++i)
        for (NoLista *p = u[i].segue.cabeca; p; p = p->prox)
            adicionarArestaLista(g, i, p->valor);
}

void exportarParaGrafoMatriz(Usuario *u, int n, GrafoMatriz *g)
{
    for (int i = 0; i < n; ++i)
        for (NoLista *p = u[i].segue.cabeca; p; p = p->prox)
            adicionarArestaMatriz(g, i, p->valor);
}


