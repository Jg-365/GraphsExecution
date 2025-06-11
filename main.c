#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "usuario.h"
#include "grafo.h"
#include "pilha.h"
#include "lista.h"
#include "rede.h"

#define REPETICOES 1.000.000

// ---------- Exportar Rede Social para Grafos ----------
void exportarParaGrafoLista(Usuario *usuarios, int n, GrafoLista *grafo) {
    for (int i = 0; i < n; i++) {
        NoLista *atual = usuarios[i].segue.cabeca;
        while (atual != NULL) {
            adicionarArestaLista(grafo, i, atual->valor);
            atual = atual->prox;
        }
    }
}

void exportarParaGrafoMatriz(Usuario *usuarios, int n, GrafoMatriz *grafo) {
    for (int i = 0; i < n; i++) {
        NoLista *atual = usuarios[i].segue.cabeca;
        while (atual != NULL) {
            adicionarArestaMatriz(grafo, i, atual->valor);
            atual = atual->prox;
        }
    }
}

// ---------- Força Bruta SCC (Lista) ----------
void forcaBrutaSCC(GrafoLista *grafo, int imprimir) {
    int n = grafo->num_vertices;
    int *alcanca = (int*)calloc(n, sizeof(int));
    int *alcancadoPor = (int*)calloc(n, sizeof(int));

    if (imprimir) printf("Componentes Fortemente Conexas (Força Bruta - Lista):\n");

    int componente = 0;
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < n; i++) alcanca[i] = alcancadoPor[i] = 0;

        DFSLista(grafo, u, alcanca);

        GrafoLista *grafoReverso = criarGrafoLista(n);
        for (int i = 0; i < n; i++) {
            NoAdj *viz = grafo->vertices[i].cabeca;
            while (viz) {
                adicionarArestaLista(grafoReverso, viz->id_vizinho, i);
                viz = viz->prox;
            }
        }
        DFSLista(grafoReverso, u, alcancadoPor);
        liberarGrafoLista(grafoReverso);

        if (imprimir) {
            printf("SCC %d: ", ++componente);
            for (int v = 0; v < n; v++) {
                if (alcanca[v] && alcancadoPor[v]) {
                    printf("%d ", v);
                }
            }
            printf("\n");
        }
    }

    free(alcanca);
    free(alcancadoPor);
}

// ---------- Força Bruta SCC (Matriz) ----------
void forcaBrutaSCC_Matriz(GrafoMatriz *grafo, int imprimir) {
    int n = grafo->num_vertices;
    int *alcanca = (int*)calloc(n, sizeof(int));
    int *alcancadoPor = (int*)calloc(n, sizeof(int));

    if (imprimir) printf("Componentes Fortemente Conexas (Força Bruta - Matriz):\n");

    int componente = 0;
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < n; i++) alcanca[i] = alcancadoPor[i] = 0;

        DFSMatriz(grafo, u, alcanca);

        GrafoMatriz *grafoReverso = criarGrafoMatriz(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grafo->matriz[i][j]) {
                    adicionarArestaMatriz(grafoReverso, j, i);
                }
            }
        }
        DFSMatriz(grafoReverso, u, alcancadoPor);
        liberarGrafoMatriz(grafoReverso);

        if (imprimir) {
            printf("SCC %d: ", ++componente);
            for (int v = 0; v < n; v++) {
                if (alcanca[v] && alcancadoPor[v]) {
                    printf("%d ", v);
                }
            }
            printf("\n");
        }
    }

    free(alcanca);
    free(alcancadoPor);
}

// ---------- Tarjan SCC (Lista) ----------
void tarjanDFS(GrafoLista *grafo, int u, int *index, int *lowlink, int *onStack, int *idx, Pilha *pilha, int *componente, int imprimir) {
    index[u] = *idx;
    lowlink[u] = *idx;
    (*idx)++;
    push(pilha, u);
    onStack[u] = 1;

    NoAdj *vizinho = grafo->vertices[u].cabeca;
    while (vizinho != NULL) {
        int v = vizinho->id_vizinho;
        if (index[v] == -1) {
            tarjanDFS(grafo, v, index, lowlink, onStack, idx, pilha, componente, imprimir);
            if (lowlink[u] > lowlink[v]) lowlink[u] = lowlink[v];
        } else if (onStack[v]) {
            if (lowlink[u] > index[v]) lowlink[u] = index[v];
        }
        vizinho = vizinho->prox;
    }

    if (lowlink[u] == index[u]) {
        if (imprimir) printf("SCC %d: ", ++(*componente));
        int w;
        do {
            w = pop(pilha);
            onStack[w] = 0;
            if (imprimir) printf("%d ", w);
        } while (w != u);
        if (imprimir) printf("\n");
    }
}

void tarjanSCC(GrafoLista *grafo, int imprimir) {
    int n = grafo->num_vertices;
    int *index = (int*)malloc(n * sizeof(int));
    int *lowlink = (int*)malloc(n * sizeof(int));
    int *onStack = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) index[i] = -1;

    Pilha pilha;
    inicializarPilha(&pilha);

    int idx = 0;
    int componente = 0;
    for (int u = 0; u < n; u++) {
        if (index[u] == -1) {
            tarjanDFS(grafo, u, index, lowlink, onStack, &idx, &pilha, &componente, imprimir);
        }
    }

    free(index);
    free(lowlink);
    free(onStack);
    liberarPilha(&pilha);
}

// ---------- Tarjan SCC (Matriz) ----------
void tarjanDFS_Matriz(GrafoMatriz *grafo, int u, int *index, int *lowlink, int *onStack, int *idx, Pilha *pilha, int *componente, int imprimir) {
    index[u] = *idx;
    lowlink[u] = *idx;
    (*idx)++;
    push(pilha, u);
    onStack[u] = 1;

    for (int v = 0; v < grafo->num_vertices; v++) {
        if (grafo->matriz[u][v]) {
            if (index[v] == -1) {
                tarjanDFS_Matriz(grafo, v, index, lowlink, onStack, idx, pilha, componente, imprimir);
                if (lowlink[u] > lowlink[v]) lowlink[u] = lowlink[v];
            } else if (onStack[v]) {
                if (lowlink[u] > index[v]) lowlink[u] = index[v];
            }
        }
    }

    if (lowlink[u] == index[u]) {
        if (imprimir) printf("SCC %d: ", ++(*componente));
        int w;
        do {
            w = pop(pilha);
            onStack[w] = 0;
            if (imprimir) printf("%d ", w);
        } while (w != u);
        if (imprimir) printf("\n");
    }
}

void tarjanSCC_Matriz(GrafoMatriz *grafo, int imprimir) {
    int n = grafo->num_vertices;
    int *index = (int*)malloc(n * sizeof(int));
    int *lowlink = (int*)malloc(n * sizeof(int));
    int *onStack = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) index[i] = -1;

    Pilha pilha;
    inicializarPilha(&pilha);

    int idx = 0;
    int componente = 0;
    for (int u = 0; u < n; u++) {
        if (index[u] == -1) {
            tarjanDFS_Matriz(grafo, u, index, lowlink, onStack, &idx, &pilha, &componente, imprimir);
        }
    }

    free(index);
    free(lowlink);
    free(onStack);
    liberarPilha(&pilha);
}

// ---------- MAIN ----------
int main() {
    int n = 500;  // Ajuste para 500, 1000 ou 2000
    Usuario* usuarios = criarUsuarios(n);
    gerarRedeSocial(usuarios, n);

    GrafoLista* grafoLista = criarGrafoLista(n);
    GrafoMatriz* grafoMatriz = criarGrafoMatriz(n);

    exportarParaGrafoLista(usuarios, n, grafoLista);
    exportarParaGrafoMatriz(usuarios, n, grafoMatriz);

    printf("\n========== RESULTADOS FINAIS ==========\n");
    printf("\nForça Bruta SCC (Lista):\n");
    forcaBrutaSCC(grafoLista, 1);

    printf("\nForça Bruta SCC (Matriz):\n");
    forcaBrutaSCC_Matriz(grafoMatriz, 1);

    printf("\nTarjan SCC (Lista):\n");
    tarjanSCC(grafoLista, 1);

    printf("\nTarjan SCC (Matriz):\n");
    tarjanSCC_Matriz(grafoMatriz, 1);

    FILE *arquivo = fopen("resultados.csv", "w");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo de resultados.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(arquivo, "Algoritmo,Representacao,TempoMedioMS\n");

    clock_t inicio, fim;
    double tempo_ms;

    inicio = clock();
    for (int rep = 0; rep < REPETICOES; rep++) {
        forcaBrutaSCC(grafoLista, 0);
    }
    fim = clock();
    tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0 / REPETICOES;
    fprintf(arquivo, "ForcaBruta,Lista,%.4f\n", tempo_ms);

    inicio = clock();
    for (int rep = 0; rep < REPETICOES; rep++) {
        forcaBrutaSCC_Matriz(grafoMatriz, 0);
    }
    fim = clock();
    tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0 / REPETICOES;
    fprintf(arquivo, "ForcaBruta,Matriz,%.4f\n", tempo_ms);

    inicio = clock();
    for (int rep = 0; rep < REPETICOES; rep++) {
        tarjanSCC(grafoLista, 0);
    }
    fim = clock();
    tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0 / REPETICOES;
    fprintf(arquivo, "Tarjan,Lista,%.4f\n", tempo_ms);

    inicio = clock();
    for (int rep = 0; rep < REPETICOES; rep++) {
        tarjanSCC_Matriz(grafoMatriz, 0);
    }
    fim = clock();
    tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0 / REPETICOES;
    fprintf(arquivo, "Tarjan,Matriz,%.4f\n", tempo_ms);

    fclose(arquivo);

    liberarUsuarios(usuarios, n);
    liberarGrafoLista(grafoLista);
    liberarGrafoMatriz(grafoMatriz);

    printf("\nBenchmark concluído! Resultados exportados para 'resultados.csv'.\n");
    return 0;
}

