#ifndef REDE_H
#define REDE_H

void gerarRedeSocial(Usuario* usarios, int n);

void exportarParaGrafoListaReverso(Usuario *u, int n, GrafoLista *gt);

void exportarParaGrafoMatrizReverso(Usuario *u, int n, GrafoMatriz *gt);

void exportarParaGrafoLista(Usuario *u, int n, GrafoLista *g);

void exportarParaGrafoMatriz(Usuario *u, int n, GrafoMatriz *g);

#endif

