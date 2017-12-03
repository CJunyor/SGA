#include <stdlib.h>
#include <stdio.h>
#include "BTree.h"


BTree criarNo(){
    BTree aux = (BTree)malloc(sizeof(NoArv));
    int i=0;

    for (i=0; i<MAXFILHOS-1; i++){
        aux->filhos[i]=NULL;
        aux->valido[i]=0;
    }

    aux->preenchidos=0;

    return aux;
}

void carregaBTree(char nome[], BTree *raiz, SEGMENT area){
    FILE *fp;
    indice buffer;
    fp = fopen(nome, "r+b");
    if (fp==NULL){
        printf("Erro ao abrir o BD");
        exit(1);
    }
    fseek(fp, area.BASE, SEEK_SET);
    while(ftell(fp)<=area.LIMIT+area.BASE){
        fread(&buffer, sizeof(indice), 1, fp);
        if (buffer.seek.BASE != -1)
            Insere(*raiz, buffer);
    }
    fclose(fp);
}

int buscaBin(BTree vetor, int preenchidos, int chave){
    int inicio;
    int fim;
    int metade;

    inicio = 0;
    fim = preenchidos-1;

    while (inicio<=fim){
        metade = (inicio+fim)/2;

        if (vetor->chave[metade].n==chave){
            return metade;
        }else if(vetor->chave[metade].n>chave){
            fim = metade-1;
        }
        else{
            inicio = metade+1;
        }
    }
    return inicio;
}

/*
int insereChave(BTree no, indice chave, BTree filhodir){
    int posIdeal, fim;

    posIdeal = buscaBin(no->chave, no->preenchidos, chave);
    fim = no->preenchidos-1;

    if(no->preenchidos<CHEIA){
        while(fim>posIdeal){
            no->chave[fim+1]=no->chave[fim];
            fim--;
        }
        no->chave[posIdeal]=chave;
        no->preenchidos++;
        no->filhos[pos+1] = filhodir;
        return 1;
    }
    printf("No cheio");
    return 0;
}*/


void  insereChave(BTree raiz, indice info, BTree filhodir)
{
  int k, pos;

  //busca para obter a posição ideal para inserir a nova chave
  pos = buscaBin(raiz, raiz->preenchidos, info.n);
  k = raiz->preenchidos;


    //realiza o remanejamento para manter as chaves ordenadas
    while (k > pos && info.n < raiz->chave[k-1].n)
    {
        raiz->chave[k].n = raiz->chave[k-1].n;
        raiz->chave[k].seek = raiz->chave[k-1].seek;
        raiz->filhos[k+1] = raiz->filhos[k];
        k--;
    }
    //insere a chave na posição ideal
    raiz->chave[pos].n = info.n;
    raiz->chave[pos].seek = info.seek;
    raiz->valido[pos]=1;
    raiz->filhos[pos+1] = filhodir;
    raiz->preenchidos++;

}


BTree Insercao(BTree raiz, indice chave, int *flag, indice *retorno)
{
    //printf("\n%d\n", chave.seek.BASE);
  indice info_mediano; //auxiliar para armazenar a chave que irá subir para o pai
  int i, pos;
      BTree temp, filho_dir; //ponteiro para o filho à direita da chave

  if (raiz == NULL)
   {
     //O nó anterior é o ideal para inserir a nova chave (chegou em um nó folha)
     *flag = 1;
     *retorno = chave;
     //printf("\n%d\n", (*retorno).seek.BASE);
     return(NULL);
   }
  else {
         pos = buscaBin(raiz, raiz->preenchidos, chave.n);
         if (raiz->preenchidos > pos && raiz->chave[pos].n == chave.n)
           {
             //  printf("\n%d\n", chave.seek.BASE);
             printf("Chave já contida na Árvore");
             *flag = 0;
           }
         else {
                               //desce na árvore até encontrar o nó folha para inserir a chave.
               filho_dir = Insercao(raiz->filhos[pos],chave,flag,retorno);
               //printf("\n%d\n", (*retorno).seek.BASE);
               if (*flag) //Se true deve inserir a info_retorno no nó.
                 {
                     //printf("\n%d\n", (*retorno).seek.BASE);
                     if (raiz->preenchidos < CHEIA) //Tem espaço na página
                     {
                       insereChave(raiz, *retorno, filho_dir);
                       *flag = 0;
                     }
                   else {
                        //printf("\n%d\n", raiz->chave[0]);
                          temp = criarNo();
                          //printf("\n%d\n", raiz->chave[3].seek.LIMIT);
                         info_mediano = raiz->chave[MIN_OCUP];
                         //printf("\n%d\n", info_mediano.seek.BASE);

                         //insere metade do nó raiz no temp (efetua subdivisão)
                         temp->filhos[0] = raiz->filhos[MIN_OCUP+1];
                         for (i = MIN_OCUP + 1; i < CHEIA; i++)
                           insereChave(temp, raiz->chave[i], filho_dir);

                         //atualiza nó raiz.
                         for (i = MIN_OCUP; i<CHEIA; i++)
                         {
                           raiz->chave[i].n = 0;
                           raiz->filhos[i+1] = NULL;
                         }
                         raiz->preenchidos = MIN_OCUP;

                         //Verifica em qual nó será inserida a nova chave
                         if (pos <= MIN_OCUP)
                              insereChave(raiz, *retorno, filho_dir);
                         else insereChave(temp, *retorno, filho_dir);

                         //retorna o mediano para inserí-lo no nó pai e o temp como filho direito do mediano.
                         *retorno = info_mediano;
                         return(temp);
                       }
                 }
              }
     }
    return (raiz);
}

BTree Insere(BTree no, indice v)
{

  int flag;
  int  i;
  indice retorno;
  BTree filho_dir, buffer;

    //printf("\n%d\n", v.seek.BASE);
  filho_dir = Insercao(no, v,&flag,&retorno);
  if (flag)
   { //Aumetará a altura da árvore
     buffer = criarNo();
     buffer->preenchidos = 1;
     buffer->chave[0] = retorno;
     buffer->filhos[0] = no;
     buffer->filhos[1] = filho_dir;
     for (i = 2; i <= CHEIA; i++)
       buffer->filhos[i] = NULL;
     return(buffer);
   }
  return(no);
}

BTree buscaChave(BTree raiz, int chave, int *position)
{
  BTree buffer;
  int pos; //posição retornada pelo busca binária.

  buffer = raiz;
  while (buffer != NULL)
   {
     pos = buscaBin(buffer, buffer->preenchidos, chave);
     if (pos < buffer->preenchidos && buffer->chave[pos].n == chave){
        *position = pos;
         return(buffer);
     }
     else{
         *position = -1;
        buffer = buffer->filhos[pos];
     }
   }
}

void em_ordem(BTree raiz)
{
  int i;

  if (raiz != NULL)
   {
     for (i = 0; i < raiz->preenchidos; i++)
      {
        em_ordem(raiz->filhos[i]);
        printf("\n%d", raiz->chave[i].n);
      }
     em_ordem(raiz->filhos[i]);
      }
}

