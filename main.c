#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "user.h"
#include "grafo.h"
#include "pilha.h"
#include "lista.h"
#include "rede.h"

#define N_THREADS 8
#define REPETICOES 1000000

typedef struct {
    int id;
    int reps;
    GrafoLista* grafoLista;
    GrafoMatriz* grafoMatriz;
    GrafoLista* grafoReversoLista;
    GrafoMatriz* grafoReversoMatriz;
    double tempo_lista_bruta;
    double tempo_matriz_bruta;
    double tempo_lista_tarjan;
    double tempo_matriz_tarjan;
} ThreadArgs;

void* benchmark_thread(void* arg) {
    ThreadArgs* t = (ThreadArgs*)arg;
    clock_t ini, fim;

    ini = clock();
    for (int i = 0; i < t->reps; i++) forcaBrutaSCC(t->grafoLista, t->grafoReversoLista, 0);
    fim = clock();
    t->tempo_lista_bruta = ((double)(fim - ini)) / CLOCKS_PER_SEC;

    ini = clock();
    for (int i = 0; i < t->reps; i++) forcaBrutaSCC_Matriz(t->grafoMatriz, t->grafoReversoMatriz, 0);
    fim = clock();
    t->tempo_matriz_bruta = ((double)(fim - ini)) / CLOCKS_PER_SEC;

    ini = clock();
    for (int i = 0; i < t->reps; i++) tarjanSCC(t->grafoLista, 0);
    fim = clock();
    t->tempo_lista_tarjan = ((double)(fim - ini)) / CLOCKS_PER_SEC;

    ini = clock();
    for (int i = 0; i < t->reps; i++) tarjanSCC_Matriz(t->grafoMatriz, 0);
    fim = clock();
    t->tempo_matriz_tarjan = ((double)(fim - ini)) / CLOCKS_PER_SEC;

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    FILE* arquivo = fopen("resultados.csv", "w");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo para salvar resultados.\n");
        return 1;
    }
    fprintf(arquivo, "n,Algoritmo,Representacao,TempoTotalSegundos,TempoMedioMS\n");

    int tamanhos[] = {500, 1000, 2000};
    for (int t = 0; t < 3; ++t) {
        int n = tamanhos[t];
        Usuario* usuarios = criarUsuarios(n);
        gerarRedeSocial(usuarios, n);

        GrafoLista* grafoLista = criarGrafoLista(n);
        GrafoMatriz* grafoMatriz = criarGrafoMatriz(n);
        GrafoLista* grafoReversoLista = criarGrafoLista(n);
        GrafoMatriz* grafoReversoMatriz = criarGrafoMatriz(n);

        exportarParaGrafoLista(usuarios, n, grafoLista);
        exportarParaGrafoMatriz(usuarios, n, grafoMatriz);
        exportarParaGrafoListaReverso(usuarios, n, grafoReversoLista);
        exportarParaGrafoMatrizReverso(usuarios, n, grafoReversoMatriz);

        pthread_t threads[N_THREADS];
        ThreadArgs args[N_THREADS];
        int base = REPETICOES / N_THREADS;
        int resto = REPETICOES % N_THREADS;

        double ini = (double)clock() / CLOCKS_PER_SEC;
        for (int i = 0; i < N_THREADS; i++) {
            args[i].id = i;
            args[i].reps = base + (i < resto ? 1 : 0);
            args[i].grafoLista = grafoLista;
            args[i].grafoMatriz = grafoMatriz;
            args[i].grafoReversoLista = grafoReversoLista;
            args[i].grafoReversoMatriz = grafoReversoMatriz;
            pthread_create(&threads[i], NULL, benchmark_thread, &args[i]);
        }

        double total_lista_bruta = 0, total_matriz_bruta = 0, total_lista_tarjan = 0, total_matriz_tarjan = 0;

        for (int i = 0; i < N_THREADS; i++) {
            pthread_join(threads[i], NULL);
            total_lista_bruta += args[i].tempo_lista_bruta;
            total_matriz_bruta += args[i].tempo_matriz_bruta;
            total_lista_tarjan += args[i].tempo_lista_tarjan;
            total_matriz_tarjan += args[i].tempo_matriz_tarjan;
        }
        double fim = (double)clock() / CLOCKS_PER_SEC;
        double tempo_total = fim - ini;

        fprintf(arquivo, "%d,ForcaBruta,Lista,%.2f,%.4f\n", n, tempo_total, (total_lista_bruta * 1000) / REPETICOES);
        fprintf(arquivo, "%d,ForcaBruta,Matriz,%.2f,%.4f\n", n, tempo_total, (total_matriz_bruta * 1000) / REPETICOES);
        fprintf(arquivo, "%d,Tarjan,Lista,%.2f,%.4f\n", n, tempo_total, (total_lista_tarjan * 1000) / REPETICOES);
        fprintf(arquivo, "%d,Tarjan,Matriz,%.2f,%.4f\n", n, tempo_total, (total_matriz_tarjan * 1000) / REPETICOES);

        liberarUsuarios(usuarios, n);
        liberarGrafoLista(grafoLista);
        liberarGrafoMatriz(grafoMatriz);
        liberarGrafoLista(grafoReversoLista);
        liberarGrafoMatriz(grafoReversoMatriz);
    }
    fclose(arquivo);

    printf("\n Benchmark finalizado. Resultados salvos em 'resultados.csv'.\n");
    return 0;
}

