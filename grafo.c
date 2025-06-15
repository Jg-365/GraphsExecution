#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "lista.h"
#include "pilha.h"

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

void forcaBrutaSCC(GrafoLista *g, GrafoLista *gt, int imprimir) {
    int *visitado1 = (int *)calloc(g->num_vertices, sizeof(int));
    int *visitado2 = (int *)calloc(g->num_vertices, sizeof(int));
    int *scc = (int *)calloc(g->num_vertices, sizeof(int));
    int componente = 1;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!scc[i]) {
            for (int j = 0; j < g->num_vertices; j++) {
                visitado1[j] = visitado2[j] = 0;
            }

            DFSLista(g, i, visitado1);
            DFSLista(gt, i, visitado2);

            for (int j = 0; j < g->num_vertices; j++) {
                if (visitado1[j] && visitado2[j] && !scc[j]) {
                    scc[j] = componente;
                }
            }
            componente++;
        }
    }

    if (imprimir) {
        printf("\n--- SCCs (Força Bruta - Lista) ---\n");
        for (int c = 1; c < componente; c++) {
            printf("Componente %d: ", c);
            for (int i = 0; i < g->num_vertices; i++) {
                if (scc[i] == c)
                    printf("%d ", i);
            }
            printf("\n");
        }
    }

    free(visitado1);
    free(visitado2);
    free(scc);
}

/* -------------------------------------------------------------
   TARJAN – LISTA DE ADJACÊNCIA
   ------------------------------------------------------------- */
static void tarjanDFSLista(GrafoLista *g, int u,
                           int *idx, int *low, int *onStack,
                           int *curIndex, Pilha *S, int imprimir, int *comp)
{
    idx[u]  = low[u] = (*curIndex)++;
    push(S, u);
    onStack[u] = 1;

    for (NoAdj *p = g->vertices[u].cabeca; p; p = p->prox) {
        int v = p->id_vizinho;
        if (idx[v] == -1) {
            tarjanDFSLista(g, v, idx, low, onStack, curIndex, S, imprimir, comp);
            if (low[u] > low[v]) low[u] = low[v];
        } else if (onStack[v] && low[u] > idx[v]) {
            low[u] = idx[v];
        }
    }
    if (low[u] == idx[u]) {                     /* raiz de SCC */
        if (imprimir) printf("SCC %d: ", ++(*comp));
        int w;
        do {
            w = pop(S);
            onStack[w] = 0;
            if (imprimir) printf("%d ", w);
        } while (w != u);
        if (imprimir) putchar('\n');
    }
}

void tarjanSCC(GrafoLista *g, int imprimir)
{
    int n = g->num_vertices;
    int *idx     = (int *)malloc(n * sizeof(int));
    int *low     = (int *)malloc(n * sizeof(int));
    int *onStack = (int *)calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) idx[i] = -1;

    Pilha S; inicializarPilha(&S);
    int cur = 0, comp = 0;

    for (int u = 0; u < n; ++u)
        if (idx[u] == -1)
            tarjanDFSLista(g, u, idx, low, onStack, &cur, &S, imprimir, &comp);

    free(idx); free(low); free(onStack);
    liberarPilha(&S);
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

void forcaBrutaSCC_Matriz(GrafoMatriz *g, GrafoMatriz *gt, int imprimir) {
    int *visitado1 = (int *)calloc(g->num_vertices, sizeof(int));
    int *visitado2 = (int *)calloc(g->num_vertices, sizeof(int));
    int *scc = (int *)calloc(g->num_vertices, sizeof(int));
    int componente = 1;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!scc[i]) {
            for (int j = 0; j < g->num_vertices; j++) {
                visitado1[j] = visitado2[j] = 0;
            }

            DFSMatriz(g, i, visitado1);
            DFSMatriz(gt, i, visitado2);

            for (int j = 0; j < g->num_vertices; j++) {
                if (visitado1[j] && visitado2[j] && !scc[j]) {
                    scc[j] = componente;
                }
            }
            componente++;
        }
    }

    if (imprimir) {
        printf("\n--- SCCs (Força Bruta - Matriz) ---\n");
        for (int c = 1; c < componente; c++) {
            printf("Componente %d: ", c);
            for (int i = 0; i < g->num_vertices; i++) {
                if (scc[i] == c)
                    printf("%d ", i);
            }
            printf("\n");
        }
    }

    free(visitado1);
    free(visitado2);
    free(scc);
}

static void tarjanDFSMatriz(GrafoMatriz *g, int u,
                            int *idx, int *low, int *onStack,
                            int *curIndex, Pilha *S, int imprimir, int *comp)
{
    idx[u]  = low[u] = (*curIndex)++;
    push(S, u);
    onStack[u] = 1;

    for (int v = 0; v < g->num_vertices; ++v) if (g->matriz[u][v]) {
        if (idx[v] == -1) {
            tarjanDFSMatriz(g, v, idx, low, onStack, curIndex, S, imprimir, comp);
            if (low[u] > low[v]) low[u] = low[v];
        } else if (onStack[v] && low[u] > idx[v]) {
            low[u] = idx[v];
        }
    }
    if (low[u] == idx[u]) {
        if (imprimir) printf("SCC %d: ", ++(*comp));
        int w;
        do {
            w = pop(S);
            onStack[w] = 0;
            if (imprimir) printf("%d ", w);
        } while (w != u);
        if (imprimir) putchar('\n');
    }
}

void tarjanSCC_Matriz(GrafoMatriz *g, int imprimir)
{
    int n = g->num_vertices;
    int *idx     = (int *)malloc(n * sizeof(int));
    int *low     = (int *)malloc(n * sizeof(int));
    int *onStack = (int *)calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) idx[i] = -1;

    Pilha S; inicializarPilha(&S);
    int cur = 0, comp = 0;

    for (int u = 0; u < n; ++u)
        if (idx[u] == -1)
            tarjanDFSMatriz(g, u, idx, low, onStack, &cur, &S, imprimir, &comp);

    free(idx); free(low); free(onStack);
    liberarPilha(&S);
}


