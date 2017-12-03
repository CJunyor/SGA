#include "BTree.h"

int main(){
BTree aux = criarNo();
indice i;
int x;

for (x=0; x<=90; x++){
    i.n=x*x;
    aux=Insere(aux, i);
}

aux=buscaChave(aux, 8100, &x);

em_ordem(aux);
printf("\n%d\n", aux->chave[x]);
return 0;

}
