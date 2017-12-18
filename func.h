#include <iostream>
#include <cstring>
#include <vector>
#define cout(x) cout<<x<<endl
#define cint(x) (int *) x
#define cchar(x) (char *) x
#define cstring(x) (string *) x
#define cfloat(x) (float *) x
#define cdouble(x) (double *) x
#define cvoid(x) (void *) x
#define INT 'i'
#define FLOAT 'f'
#define DOUBLE 'd'
#define CHAR 'c'
#define STRING 's'
#define BIT_PROTETOR 'û'
#define ENTRADA 'I'
#define SAIDA 'O'
#define ENTRADA_SAIDA 'Î'

#include <stdio.h>
#include <stdlib.h>
#define CHEIA 4
#define MAXFILHOS 5
#define MIN_OCUP 2


using namespace std;

//=============================================================================================

typedef struct{
    string nome;
    void *info;
}node;

typedef struct{
    string nome;
    vector<node> vars;
}MEM_REGISTER;

//=============================================================================================

typedef struct{
    int BASE;
    int LIMIT;
}SEGMENT;


typedef struct{
    int n;
    SEGMENT seek;
}indice;

typedef struct{
    SEGMENT DATA_POS;
    SEGMENT NEXT;
}DATA;

typedef struct{
    int NUMERO_COLUNAS;
    int NUMERO_REGISTROS;
    SEGMENT TABLE_POSITION;
    SEGMENT NEXT;
}TABLE_HEADER;

typedef struct{
    int NUMERO_TABELAS;
}DATABASE_HEADER;

typedef struct{
    SEGMENT NOME;
    SEGMENT TIPOS;
    DATA NOMES;
}META_DADO;

typedef struct{
    DATA DADOS;
    SEGMENT INDEX;
    SEGMENT NEXT;
}REGISTRO;

typedef struct{
    int ID_ATUAL;
    SEGMENT INDEX_AREA;
    SEGMENT INDICES;
    SEGMENT META_TABLE_POSITION;
    SEGMENT REGISTROS;
}TABLE;

typedef struct{
    string NOME;
    //bool OPEN;
    DATABASE_HEADER BASE_HEADER;
    SEGMENT TABELAS_HEADER_SEG;
    SEGMENT SEGMENTOS_LIVRES;
}DATABASE;


//=================================================================================
bool CONTROL(DATABASE *, vector<string>[]);
bool CRIAR_DATABASE(string, vector<string>[], int);
bool CRIAR_DATABASE_VAZIO(string);
bool FILL_TABLE_HEADER(fstream *, TABLE_HEADER *, int);
bool FILL_TABLE(fstream *, TABLE *, TABLE_HEADER *);
bool FILL_MDADO(fstream *, META_DADO *, TABLE_HEADER *, vector<string> *, DATA []);
bool FILL_DADOS(fstream *, TABLE_HEADER *, DATA []);
bool SET_BLOCK(fstream *, char *, SEGMENT);
bool SET_DB_HEADER(fstream *, DATABASE *, bool);
bool SET_TABLE_HEADER(fstream *, TABLE_HEADER *, SEGMENT);
bool SET_TABLE(fstream *, TABLE *, SEGMENT);
bool SET_MDADO(fstream *, META_DADO *, TABLE_HEADER *, TABLE *, DATA[]);
bool SET_N_TABLES(fstream *, DATABASE *);
bool SET_REGISTER(fstream *, REGISTRO  *, SEGMENT);
bool SET_DADOS(fstream *, TABLE_HEADER *, DATA []);
bool SET_DADO(fstream *, DATA *, vector<string> *, int, void *, SEGMENT);
bool SET_COLUNA(fstream *, DATABASE *, META_DADO *, TABLE *, string, string);
bool SET_INDEX(fstream *, TABLE *, TABLE_HEADER *, REGISTRO *, SEGMENT);
bool INSERIR_REGISTRO(DATABASE *, MEM_REGISTER);
bool UPDATE_REGISTRO(DATABASE *, string [4], string);
bool DELETAR_REGISTRO(DATABASE *, string [3], void **);
bool DELETAR_DADOS(fstream *, DATABASE *, DATA *);
bool INSERIR_COLUNA(DATABASE *, string, string, char);
bool INSERIR_TABELA(DATABASE *, vector<string> *);
bool INSERIR_TABELA_VAZIA(DATABASE *, string);
bool LER_TABLE_HEADER(fstream *, TABLE_HEADER *, SEGMENT);
bool LER_TABELA(fstream *, TABLE *, SEGMENT);
bool LER_MDADO(fstream *, META_DADO *, TABLE *);
bool LER_REGISTRO(fstream *, REGISTRO *, SEGMENT);
bool LER_DADO(fstream *, DATA *, SEGMENT);
bool RtoMR(fstream *, vector<string> *, REGISTRO *, MEM_REGISTER *);


vector<MEM_REGISTER> PEGAR_REGISTRO(DATABASE *, vector<string> *);
bool MDADO_TO_STRING(META_DADO *, vector<string> *, fstream *);
SEGMENT SEARCH_TABLE(fstream *f, DATABASE *db, TABLE_HEADER *TH, TABLE *T, META_DADO *M, string nome);
string PEGAR_REG_TIPO(DATABASE *, string);
SEGMENT PEGAR_TABLE_INDEX(DATABASE *, TABLE_HEADER *, TABLE *, META_DADO *, string);
fstream *OPEN_DATABASE (DATABASE *);
SEGMENT Dallocar(fstream *, int);
int TSize(char);
bool GET_BLOCK(fstream *, char *, SEGMENT);
void *Malocar(char);
bool Atribuir(node *, string, char);
void IMPRIMIR_MR(vector<MEM_REGISTER> *, string);
bool COMPARAR(void *, void *, char);
bool COMPARAR_VOID_STRING(void *, string, char);
void FREE_MR(vector<MEM_REGISTER> *, string);
bool ABRIR_ARQUIVO(fstream *, string, char);
bool VALID_SEG(SEGMENT);
bool DDESALOCAR(fstream *, DATABASE *, SEGMENT);
//=================================================================================

void Desaloc(char, void *);





typedef struct NoArv{
    int preenchidos;
    indice chave[CHEIA];
    struct NoArv *filhos[MAXFILHOS];
    int valido[CHEIA];
}NoArv, *BTree;

BTree criarNo();
// Cria um nó vazio, inicializado.

void carregaBTree(char nome[], BTree *raiz, SEGMENT area);
//carrega a arvore com os indices presentes no arquivo do BD

int buscaBin(BTree vetor, int preenchidos, int chave);
// Realiza uma busca binária em um vetor do nó da BTree, retornando a posição no vetor do nó, caso encontrado
// ou retornando a posição no vetor de filhos para continuar a busca

void  insereChave(BTree raiz, indice info, BTree filhodir);
// Dado um nó da BTree, insere o indice nesse nó.

BTree Insercao(BTree raiz, indice chave, int *flag, indice *retorno);
// Função intermediária para inserção.
// Insere um nó na BTree, procurando primeiramente qual o nó ideal para se inserir, ou dividindo o nó em dois
// caso se faça necessário. flag informa se existe se ainda existe a necessidade de inserção, essa informação é usada
// na recursividade e na função insere. Chave é o indice que se deseja inserir, e retorno pode ser o proprio indice ou
// o um indice especial contendo o valor mediano que será promovido para o nó de cima.


BTree Insere(BTree no, indice v);
// Função de inserção propriamente dita, onde se passa a raiz e o indice e ela insere na BTree
// Foi necessária pois pode acontecer de a subdivisão se fazer necessária na raiz, em outras palavras
// O indice precisar ser inserido na raiz mas ela estar cheia.

BTree buscaChave(BTree raiz, int chave, int *position);
// Busca uma chave dentro da arvore, retornando o nó que contem a chave (ou nulo caso não ache)
// Caso ache o valor, 'pos' recebe o deslogamento dentro do no de retorno onde o dado se encontra

void em_ordem(BTree raiz);
// Printa os valores da BTree de forma crescente, baseada nas funções do site da ifmg.

int salvaBTree(char nome[], BTree *raiz, SEGMENT seg, int seek);
//PARÂMETROS: NOME DO BD, RAIZ DA BTREE, SEGMENT DOS INDICES, A BASE DO SEGMENT QUE VC TA PASSANDO (.BASE)
