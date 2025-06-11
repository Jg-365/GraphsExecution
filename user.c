#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "lista.h"

// Cria vetor de usu�rios e inicializa as listas
Usuario* criarUsuarios(int n) {
    Usuario *usuarios = (Usuario *)malloc(n * sizeof(Usuario));
    if (!usuarios) {
        fprintf(stderr, "Erro de aloca��o de mem�ria para usu�rios.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        usuarios[i].id = i;
        inicializarLista(&usuarios[i].segue);
        inicializarLista(&usuarios[i].seguidoPor);
    }

    return usuarios;
}

// Adiciona uma rela��o de seguimento (origem segue destino)
void adicionarSeguido(Usuario *usuarios, int id_origem, int id_destino) {
    // Adiciona o destino � lista 'segue' do usu�rio origem
    inserirLista(&usuarios[id_origem].segue, id_destino);

    // Adiciona o origem � lista 'seguidoPor' do usu�rio destino
    inserirLista(&usuarios[id_destino].seguidoPor, id_origem);
}

// Libera todas as listas e o vetor de usu�rios
void liberarUsuarios(Usuario *usuarios, int n) {
    for (int i = 0; i < n; i++) {
        liberarLista(&usuarios[i].segue);
        liberarLista(&usuarios[i].seguidoPor);
    }
    free(usuarios);
}

