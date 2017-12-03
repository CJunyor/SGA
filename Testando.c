#include "BTree.h"

int main(){
BTree aux = criarNo();
indice i;
int x;

for (x=0; x<=31; x++){
    i.n=2*x;
    aux=Insere(aux, i);
}
i.n=62;
aux=Insere(aux,i);
//aux=buscaChave(aux, 8100, &x);

em_ordem(aux);
printf("\n%d\n", aux->preenchidos);
return 0;

}
