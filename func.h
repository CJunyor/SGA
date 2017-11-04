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
#define ENTRADA 'I'
#define SAIDA 'O'
#define ENTRADA_SAIDA 'Î'

using namespace std;
//=============================================================================================
/*
typedef struct MTD;
typedef struct no;
typedef struct tabela;

//=============================================================================================
typedef struct SEGMENT;
typedef struct TABLE_HEADER;
typedef struct DATABASE_HEADER;
typedef struct M_DADO;
typedef struct REGISTRO;
typedef struct TABLE;
typedef struct DATABASE;
*/
//=============================================================================================
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
    SEGMENT *NOMES;
}M_DADO;

typedef struct{
    SEGMENT *DADOS;
    SEGMENT NEXT;
}REGISTRO;

typedef struct{
    SEGMENT META_TABLE_POSITION;
    SEGMENT REGISTROS;
}TABLE;

typedef struct{
    string NOME;
    //bool OPEN;
    DATABASE_HEADER BASE_HEADER;
    SEGMENT TABELAS_HEADER_SEG;
}DATABASE;


//=================================================================================
bool CONTROL(DATABASE *, vector<string>[]);
DATABASE *CRIAR_DATABASE(string, vector<string>[], int);
bool INSERIR_REGISTRO(DATABASE *, MEM_REGISTER);
bool LER_TABLE_HEADER(fstream *, TABLE_HEADER *, SEGMENT);
bool LER_TABELA(fstream *, TABLE *, SEGMENT);
bool LER_MDADO(fstream *, M_DADO *, TABLE_HEADER *, SEGMENT);
bool LER_REGISTRO(fstream *, REGISTRO *, TABLE_HEADER *, SEGMENT);
bool RtoMR(fstream *, string *, REGISTRO *, MEM_REGISTER *);
vector<MEM_REGISTER> PEGAR_REGISTRO(DATABASE *, string [3], void **);
string *MDADO_TO_STRING(M_DADO *, int, fstream *);
fstream *OPEN_DATABASE (DATABASE *);
SEGMENT *Dallocar(fstream *, int);
int TSize(char);
bool GET_BLOCK(fstream *, char *, SEGMENT);
void *Malocar(char);
void IMPRIMIR_MR(vector<MEM_REGISTER> *, string);
bool COMPARAR(void *, void *, char);
bool ABRIR_ARQUIVO(fstream *, string, char);
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
