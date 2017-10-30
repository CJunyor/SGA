#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cstring>
#include <string>
#include <list>
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

using namespace std;

typedef struct{
    char tipo;
    string nome;
}MTD;

typedef struct{
    MTD dado;
    vector<void*>info;
}no;

typedef struct{
    string nome;
    list<no>colunas;
    int idmax;
}tabela;

//=============================================================================================
typedef struct{
    int BASE;
    int LIMIT;
}SEGMENT;

typedef struct{
    int NUMERO_COLUNAS;
    int NUMERO_REGISTROS;
    SEGMENT TABLE_POSITION;
}TABLE_HEADER;

typedef struct{
    int NUMERO_TABELAS;
}DATABASE_HEADER;


typedef struct{
    SEGMENT NOME;
    SEGMENT TIPOS;
    SEGMENT *NOMES;
}M_DADO;

typedef struct{
    SEGMENT *DADOS;
    SEGMENT NEXT;
}REGISTRO;

typedef struct{
    //M_DADO META;
    //TABLE_HEADER HEADER;
    SEGMENT META_TABLE_POSITION;
    SEGMENT REGISTROS;
}TABLE;

typedef struct{
    string NOME;
    DATABASE_HEADER BASE_HEADER;
    SEGMENT *TABELAS_HEADER_SEG;
}DATABASE;


//=================================================================================
bool CONTROL(DATABASE *, vector<string>[]);
DATABASE *CRIAR_DATABASE(string, vector<string>[], int);
//=================================================================================
void ImprimeMenu();
void ImprimeTabela(tabela *);
void *Mallocar(char, void*);
void CriarTabela(tabela *);
void Atribuir(tabela *);
void Salvar(tabela *);
int Abrir(tabela *);
int Iniciar(tabela *);
int SubMenu(tabela *);
void ImprimeSubMenu();
int DSize(no *);
