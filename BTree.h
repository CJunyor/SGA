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
// Cria um n� vazio, inicializado.

void carregaBTree(char nome[], BTree *raiz, SEGMENT area);

int buscaBin(indice vetor[], int preenchidos, int chave);
// Realiza uma busca bin�ria em um vetor do n� da BTree, retornando a posi��o no vetor do n�, caso encontrado
// ou retornando uma posi��o que extrapola o tamanho do vetor caso n�o encontre

void  insereChave(BTree raiz, indice info, BTree filhodir);
// Dado um n� da BTree, insere o indice nesse n�.

BTree Insercao(BTree raiz, indice chave, bool *flag, indice *retorno);
// Fun��o intermedi�ria para inser��o.
// Insere um n� na BTree, procurando primeiramente qual o n� ideal para se inserir, ou dividindo o n� em dois
// caso se fa�a necess�rio. flag informa se existe se ainda existe a necessidade de inser��o, essa informa��o � usada
// na recursividade e na fun��o insere. Chave � o indice que se deseja inserir, e retorno pode ser o proprio indice ou
// o um indice especial contendo o valor mediano que ser� promovido para o n� de cima.


BTree Insere(BTree no, indice v);
// Fun��o de inser��o propriamente dita, onde se passa a raiz e o indice e ela insere na BTree
// Foi necess�ria pois pode acontecer de a subdivis�o se fazer necess�ria na raiz, em outras palavras
// O indice precisar ser inserido na raiz mas ela estar cheia.


void em_ordem(BTree raiz);
// Printa os valores da BTree de forma crescente, baseada nas fun��es do site da ifmg.


