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
bool CRIAR_DATABASE(string, vector<string>[], int);
bool FILL_TABLE_HEADER(fstream *, TABLE_HEADER *, int);
bool FILL_TABLE(fstream *, TABLE *, TABLE_HEADER *);
bool FILL_MDADO(fstream *, META_DADO *, TABLE_HEADER *, vector<string> *, DATA []);
bool FILL_DADOS(fstream *, TABLE_HEADER *, DATA []);
bool SET_BLOCK(fstream *, char *, SEGMENT);
bool SET_DB_HEADER(fstream *, DATABASE *);
bool SET_TABLE_HEADER(fstream *, TABLE_HEADER *, SEGMENT);
bool SET_TABLE(fstream *, TABLE *, SEGMENT);
bool SET_MDADO(fstream *, META_DADO *, TABLE_HEADER *, TABLE *, DATA[]);
bool SET_N_TABLES(fstream *, DATABASE *);
bool SET_REGISTER(fstream *, REGISTRO  *, SEGMENT);
bool SET_DADOS(fstream *, TABLE_HEADER *, DATA []);
bool SET_COLUNA(fstream *, DATABASE *, META_DADO *, TABLE *, string, string);
bool INSERIR_REGISTRO(DATABASE *, MEM_REGISTER);
bool INSERIR_COLUNA(DATABASE *, string, string, char);
bool INSERIR_TABELA(DATABASE *, vector<string> *);
bool LER_TABLE_HEADER(fstream *, TABLE_HEADER *, SEGMENT);
bool LER_TABELA(fstream *, TABLE *, SEGMENT);
bool LER_MDADO(fstream *, META_DADO *, TABLE *);
bool LER_REGISTRO(fstream *, REGISTRO *, SEGMENT);
bool RtoMR(fstream *, string *, REGISTRO *, MEM_REGISTER *);



vector<MEM_REGISTER> PEGAR_REGISTRO(DATABASE *, string [3], void **);
bool MDADO_TO_STRING(META_DADO *, int, fstream *);
SEGMENT SEARCH_TABLE(fstream *, DATABASE *, string, SEGMENT);
fstream *OPEN_DATABASE (DATABASE *);
SEGMENT Dallocar(fstream *, int);
int TSize(char);
bool GET_BLOCK(fstream *, char *, SEGMENT);
void *Malocar(char);
void IMPRIMIR_MR(vector<MEM_REGISTER> *, string);
bool COMPARAR(void *, void *, char);
bool ABRIR_ARQUIVO(fstream *, string, char);
bool VALID_SEG(SEGMENT);
bool DDESALOCAR(fstream *, DATABASE *, SEGMENT);
//=================================================================================
void ImprimeMenu();
void ImprimeSubMenu();
void *Mallocar(char, void*);
void *Alloc(char);
void Desaloc(char, void *);
