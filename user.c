#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "lista.h"

// Cria vetor de usuários e inicializa as listas
Usuario* criarUsuarios(int n) {
    Usuario *usuarios = (Usuario *)malloc(n * sizeof(Usuario));
    if (!usuarios) {
        fprintf(stderr, "Erro de alocação de memória para usuários.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        usuarios[i].id = i;
        inicializarLista(&usuarios[i].segue);
        inicializarLista(&usuarios[i].seguidoPor);
    }

    return usuarios;
}

// Adiciona uma relação de seguimento (origem segue destino)
void adicionarSeguido(Usuario *usuarios, int id_origem, int id_destino) {
    // Adiciona o destino à lista 'segue' do usuário origem
    inserirLista(&usuarios[id_origem].segue, id_destino);

    // Adiciona o origem à lista 'seguidoPor' do usuário destino
    inserirLista(&usuarios[id_destino].seguidoPor, id_origem);
}

// Libera todas as listas e o vetor de usuários
void liberarUsuarios(Usuario *usuarios, int n) {
    for (int i = 0; i < n; i++) {
        liberarLista(&usuarios[i].segue);
        liberarLista(&usuarios[i].seguidoPor);
    }
    free(usuarios);
}

