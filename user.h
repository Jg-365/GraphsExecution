#ifndef USER_H
#define USER_H

#include "lista.h"  // Importa a TAD Lista

// Estrutura para o usu�rio da rede social
typedef struct Usuario {
    int id;
    Lista segue;         // Lista de IDs que ele segue
    Lista seguidoPor;    // Lista de IDs de quem o segue
} Usuario;

// Fun��es
Usuario* criarUsuarios(int n);
void adicionarSeguido(Usuario *usuarios, int id_origem, int id_destino);
void liberarUsuarios(Usuario *usuarios, int n);

#endif

