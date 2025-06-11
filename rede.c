#include <stdio.h>
#include <stdlib.h>
#include "user.h"


void gerarRedeSocial(Usuario* usarios, int n){
	srand(time(NULL));
	int i, k;
	for(i = 0; i < n; i++){
		int num_seguidos = i + rand() %((int)log2(n)+1);
		for(k = 0; k < num_seguidos;k++){
			int j;
			do{
				j = rand() % n;
			}while(j==i);
			adicionarSeguido(usuarios, i, j);
		}
	}
	
	for(i = 0; i<n; i++){
		if(usuarios[i].seguidoPor.cabeca==NULL){
			int j;
			do{
				j = rand % n;
			}while(j==i);
		adicionarSeguido(usuarios,j,i);
		}
	}
}


