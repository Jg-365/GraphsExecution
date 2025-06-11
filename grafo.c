#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "lista.h"

// -------------------------------------------------------------
// IMPLEMENTAÇÃO PARA LISTA DE ADJACÊNCIA
// -------------------------------------------------------------

GrafoLista* criarGrafoLista(int num_vertices) {
    GrafoLista *g = (GrafoLista *)malloc(sizeof(GrafoLista));
    g->num_vertices = num_vertices;
    g->vertices = (VerticeLista *)malloc(num_vertices * sizeof(VerticeLista));
    for (int i = 0; i < num_vertices; i++) {
        g->vertices[i].id = i;
        g->vertices[i].cabeca = NULL;
    }
    return g;
}

void adicionarArestaLista(GrafoLista *g, int origem, int destino) {
    NoAdj *novoNo = (NoAdj *)malloc(sizeof(NoAdj));
    novoNo->id_vizinho = destino;
    novoNo->prox = g->vertices[origem].cabeca;
    g->vertices[origem].cabeca = novoNo;
}

void liberarGrafoLista(GrafoLista *g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        NoAdj *atual = g->vertices[i].cabeca;
        while (atual != NULL) {
            NoAdj *remover = atual;
            atual = atual->prox;
            free(remover);
        }
    }
    free(g->vertices);
    free(g);
}

void DFSLista(GrafoLista *g, int origem, int *visitado) {
    visitado[origem] = 1;
    NoAdj *vizinho = g->vertices[origem].cabeca;
    while (vizinho != NULL) {
        if (!visitado[vizinho->id_vizinho]) {
            DFSLista(g, vizinho->id_vizinho, visitado);
        }
        vizinho = vizinho->prox;
    }
}

void BFSLista(GrafoLista *g, int origem, int *visitado) {
    Lista fila;
    inicializarLista(&fila);
    inserirLista(&fila, origem);
    visitado[origem] = 1;

    while (fila.tamanho > 0) {
        int atual = fila.cabeca->valor;
        NoAdj *vizinho = g->vertices[atual].cabeca;
        NoLista *remover = fila.cabeca;
        fila.cabeca = fila.cabeca->prox;
        free(remover);
        fila.tamanho--;

        while (vizinho != NULL) {
            if (!visitado[vizinho->id_vizinho]) {
                visitado[vizinho->id_vizinho] = 1;
                inserirLista(&fila, vizinho->id_vizinho);
            }
            vizinho = vizinho->prox;
        }
    }
}

// -------------------------------------------------------------
// IMPLEMENTAÇÃO PARA MATRIZ DE ADJACÊNCIA
// -------------------------------------------------------------

GrafoMatriz* criarGrafoMatriz(int num_vertices) {
    GrafoMatriz *g = (GrafoMatriz *)malloc(sizeof(GrafoMatriz));
    g->num_vertices = num_vertices;
    g->matriz = (int **)malloc(num_vertices * sizeof(int *));
    for (int i = 0; i < num_vertices; i++) {
        g->matriz[i] = (int *)calloc(num_vertices, sizeof(int));
    }
    return g;
}

void adicionarArestaMatriz(GrafoMatriz *g, int origem, int destino) {
    g->matriz[origem][destino] = 1;
}

void liberarGrafoMatriz(GrafoMatriz *g) {
    if (!g) return;
    for (int i = 0; i < g->num_vertices; i++) {
        free(g->matriz[i]);
    }
    free(g->matriz);
    free(g);
}

void DFSMatriz(GrafoMatriz *g, int origem, int *visitado) {
    visitado[origem] = 1;
    for (int j = 0; j < g->num_vertices; j++) {
        if (g->matriz[origem][j] && !visitado[j]) {
            DFSMatriz(g, j, visitado);
        }
    }
}

void BFSMatriz(GrafoMatriz *g, int origem, int *visitado) {
    Lista fila;
    inicializarLista(&fila);
    inserirLista(&fila, origem);
    visitado[origem] = 1;

    while (fila.tamanho > 0) {
        int atual = fila.cabeca->valor;
        NoLista *remover = fila.cabeca;
        fila.cabeca = fila.cabeca->prox;
        free(remover);
        fila.tamanho--;

        for (int j = 0; j < g->num_vertices; j++) {
            if (g->matriz[atual][j] && !visitado[j]) {
                visitado[j] = 1;
                inserirLista(&fila, j);
            }
        }
    }
}

