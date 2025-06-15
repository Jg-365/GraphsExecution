#ifndef GRAFO_H
#define GRAFO_H

// ---------- LISTA DE ADJACÊNCIA ----------
typedef struct NoAdj {
    int id_vizinho;
    struct NoAdj *prox;
} NoAdj;

typedef struct VerticeLista {
    int id;
    NoAdj *cabeca;
} VerticeLista;

typedef struct GrafoLista {
    int num_vertices;
    VerticeLista *vertices;   // Vetor de vértices
} GrafoLista;

// Funções (assinaturas) para GrafoLista
GrafoLista* criarGrafoLista(int num_vertices);
void adicionarArestaLista(GrafoLista *g, int origem, int destino);
void liberarGrafoLista(GrafoLista *g);

// DFS e BFS para Lista de Adjacência
void DFSLista(GrafoLista *g, int origem, int *visitado);
void BFSLista(GrafoLista *g, int origem, int *visitado);
void forcaBrutaSCC(GrafoLista *g, GrafoLista *gt, int imprimir);

/* Algoritmo de Tarjan para SCC */
void tarjanSCC(GrafoLista *g, int imprimir);


// ---------- MATRIZ DE ADJACÊNCIA ----------
typedef struct GrafoMatriz {
    int num_vertices;
    int **matriz;             // matriz[i][j] == 1 se existe aresta de i para j
} GrafoMatriz;

// Funções (assinaturas) para GrafoMatriz
GrafoMatriz* criarGrafoMatriz(int num_vertices);
void adicionarArestaMatriz(GrafoMatriz *g, int origem, int destino);
void liberarGrafoMatriz(GrafoMatriz *g);

// DFS e BFS para Matriz de Adjacência
void DFSMatriz(GrafoMatriz *g, int origem, int *visitado);
void BFSMatriz(GrafoMatriz *g, int origem, int *visitado);
void forcaBrutaSCC_Matriz(GrafoMatriz *g, GrafoMatriz *gt, int imprimir);

/* Algoritmo de Tarjan para SCC */
void tarjanSCC_Matriz(GrafoMatriz *g, int imprimir);

#endif

