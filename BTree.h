#include <stdio.h>
#include <stdlib.h>

#define ORDEM 2
#define CHEIA 3
#define MAXFILHOS 4
#define MIN_OCUP 1

#define false 0
#define true 1

typedef int bool;

typedef struct{
    int BASE;
    int LIMIT;
}SEGMENT;

typedef struct{
    int n;
    SEGMENT seek;
}indice;

typedef struct NoArv{
    int preenchidos;
    indice chave[CHEIA];
    struct NoArv *filhos[MAXFILHOS];
}NoArv, *BTree;

BTree criarNo();
// Cria um nó vazio, inicializado.

void carregaBTree(char nome[], BTree *raiz, SEGMENT area);

int buscaBin(indice vetor[], int preenchidos, int chave);
// Realiza uma busca binária em um vetor do nó da BTree, retornando a posição no vetor do nó, caso encontrado
// ou retornando uma posição que extrapola o tamanho do vetor caso não encontre

void  insereChave(BTree raiz, indice info, BTree filhodir);
// Dado um nó da BTree, insere o indice nesse nó.

BTree Insercao(BTree raiz, indice chave, bool *flag, indice *retorno);
// Função intermediária para inserção.
// Insere um nó na BTree, procurando primeiramente qual o nó ideal para se inserir, ou dividindo o nó em dois
// caso se faça necessário. flag informa se existe se ainda existe a necessidade de inserção, essa informação é usada
// na recursividade e na função insere. Chave é o indice que se deseja inserir, e retorno pode ser o proprio indice ou
// o um indice especial contendo o valor mediano que será promovido para o nó de cima.


BTree Insere(BTree no, indice v);
// Função de inserção propriamente dita, onde se passa a raiz e o indice e ela insere na BTree
// Foi necessária pois pode acontecer de a subdivisão se fazer necessária na raiz, em outras palavras
// O indice precisar ser inserido na raiz mas ela estar cheia.


void em_ordem(BTree raiz);
// Printa os valores da BTree de forma crescente, baseada nas funções do site da ifmg.


