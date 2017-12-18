#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include "func.h"


void Desaloc(char c, void *p){
    if(c == INT) delete cint(p);
    else if(c == CHAR) delete cchar(p);
    else if(c == FLOAT) delete cfloat(p);
    else if(c == DOUBLE) delete cdouble(p);
    else if(c == STRING) delete cstring(p);
}

bool DDESALOCAR(fstream *f, DATABASE *db, SEGMENT S){
    if(VALID_SEG(S)){
        DATA D;
        SEGMENT ax, ax2;
        D.DATA_POS = S;
        D.NEXT = db->SEGMENTOS_LIVRES;
        ax = Dallocar(f, sizeof(DATA));
        if(!SET_BLOCK(f, cchar(&D), ax)) return false;
        ax2.BASE = 1 + sizeof(int) + sizeof(SEGMENT);
        ax2.LIMIT = sizeof(SEGMENT);
        if(!SET_BLOCK(f, cchar(&ax), ax2)) return false;
    }
    return true;
}

//=======================================================================================================

int TSize(char TIPO){
    if(TIPO == INT) return sizeof(int);
    else if(TIPO == CHAR) return sizeof(char);
    else if(TIPO == FLOAT) return sizeof(float);
    else if(TIPO == DOUBLE) return sizeof(double);
    else return -1;
}

SEGMENT Dallocar(fstream *f, int TAM){
    //Aloca um espaço no final do arquivo
    SEGMENT S;
    if(TAM>0){
        char lixo[TAM];
        (*f).seekp(0, fstream::end);
        S.BASE=(*f).tellp();
        S.LIMIT=TAM;
        (*f).write(cchar(lixo), TAM);
    }else{
        S.BASE=-1;
        S.LIMIT=-1;
    }
    return S;
}

bool FILL_TABLE_HEADER(fstream *f, TABLE_HEADER *TH, int nc){
    TH->NUMERO_COLUNAS = nc;
    TH->NUMERO_REGISTROS = 0;
    TH->TABLE_POSITION = Dallocar(f, sizeof(TABLE));
    TH->NEXT = Dallocar(f, sizeof(TABLE_HEADER));
    return true;
}

bool FILL_NEW_TABLE_HEADER(fstream *f, TABLE_HEADER *TH, int nc){
    TH->NUMERO_COLUNAS = nc;
    TH->NUMERO_REGISTROS = 0;
    TH->TABLE_POSITION = Dallocar(f, sizeof(TABLE));
    TH->NEXT.BASE = TH->NEXT.LIMIT = -1;
    return true;
}

bool FILL_TABLE(fstream *f, TABLE *T, TABLE_HEADER *TH){
    T->ID_ATUAL = 0;
    T->META_TABLE_POSITION = Dallocar(f, sizeof(META_DADO));
    T->INDEX_AREA = Dallocar(f, sizeof(indice) * 100);
    T->INDICES = T->INDEX_AREA;
    T->INDICES.LIMIT = 0;
    T->REGISTROS.BASE = T->REGISTROS.LIMIT = -1;
    return true;
}

bool FILL_MDADO(fstream *f, META_DADO *M, TABLE_HEADER *TH, vector<string> *v, DATA D[]){
    M->NOME = Dallocar(f, (*v)[0].size());
    if(!SET_BLOCK(f, cchar((*v)[0].c_str()), M->NOME)){
        cout("Erro ao preencher Meta Dado");
        return false;
    }
    M->TIPOS = Dallocar(f, (*v)[2].size());
    if(!SET_BLOCK(f, cchar((*v)[2].c_str()), M->TIPOS)){
        cout("Erro ao preencher Meta Dado");
        return false;
    }
    for(int i=0; i<TH->NUMERO_COLUNAS; i++){
        D[i].DATA_POS = Dallocar(f, (*v)[i+3].size());
        if(!SET_BLOCK(f, cchar((*v)[i+3].c_str()), D[i].DATA_POS)){
            cout("Erro ao preencher Meta Dado");
            return false;
        }
        if(i != TH->NUMERO_COLUNAS-1) D[i].NEXT = Dallocar(f, sizeof(DATA));
    }
    D[TH->NUMERO_COLUNAS-1].NEXT.BASE = D[TH->NUMERO_COLUNAS-1].NEXT.LIMIT = -1;
    M->NOMES = D[0];
    return true;
}

bool FILL_DADOS(fstream *f, TABLE_HEADER *TH, DATA DADOS[]){
    for(int i=0; i<TH->NUMERO_COLUNAS-1; i++){
        DADOS[i].DATA_POS.BASE = DADOS[i].DATA_POS.LIMIT = -1;
        DADOS[i].NEXT = Dallocar(f, sizeof(DATA));
    }
    DADOS[TH->NUMERO_COLUNAS-1].DATA_POS.BASE = DADOS[TH->NUMERO_COLUNAS-1].DATA_POS.LIMIT = -1;
    DADOS[TH->NUMERO_COLUNAS-1].NEXT = DADOS[TH->NUMERO_COLUNAS-1].DATA_POS;
    return true;
}

bool GET_BLOCK(fstream *f, char *p, SEGMENT S){
    if(!VALID_SEG(S)) return false;
    (*f).seekg(S.BASE);
    (*f).read(p, S.LIMIT);
    return true;
}

bool SET_BLOCK(fstream *f, char *p, SEGMENT S){
    if(!VALID_SEG(S)) return false;
    (*f).seekp(S.BASE);
    (*f).write(p, S.LIMIT);
    if((*f).bad()){
        cout("ERRO no set block finalizando");
        exit(1);
    }
    return true;
}

bool SET_DB_HEADER(fstream *f, DATABASE *db, bool vazio){
    char BIT = BIT_PROTETOR;
    SEGMENT S[2];
    (*f).write(cchar(&BIT), 1);
    (*f).write(cchar(&db->BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    S[0] = Dallocar(f, sizeof(SEGMENT));
    S[1] = Dallocar(f, sizeof(SEGMENT));
    if(vazio){
        db->TABELAS_HEADER_SEG.BASE = db->TABELAS_HEADER_SEG.LIMIT = -1;
    }
    else db->TABELAS_HEADER_SEG = Dallocar(f, sizeof(TABLE_HEADER));
    if(!SET_BLOCK(f, cchar(&db->TABELAS_HEADER_SEG), S[0])) return false;
    if(!SET_BLOCK(f, cchar(&db->SEGMENTOS_LIVRES), S[1])) return false;
    return true;
}

bool SET_TABLE_HEADER(fstream *f, TABLE_HEADER *TH, SEGMENT S){
    if(!SET_BLOCK(f, cchar(TH), S)){
        cout("Erro ao inserir Cabeçalho da TabelaTabela");
        return false;
    }
    cout("SEG BASE: "<<S.BASE);
    cout("SEG LIMIT: "<<S.LIMIT);
    cout("TABELA BASE: "<<TH->TABLE_POSITION.BASE);
    cout("TABELA LIMIT: "<<TH->TABLE_POSITION.LIMIT);
    return true;
}

bool SET_TABLE(fstream *f, TABLE *T, SEGMENT S){
    if(!SET_BLOCK(f, cchar(T), S)){
        cout("Erro ou inserir Tabela");
        return false;
    }
    return true;
}

bool SET_MDADO(fstream *f, META_DADO *M, TABLE_HEADER *TH, TABLE *T, DATA D[]){
    if(!SET_BLOCK(f, cchar(M), T->META_TABLE_POSITION)){
        cout("Ocorreu um erro ao tente inserir o Meta Dado");
        return false;
    }
    for(int i=1; i<TH->NUMERO_COLUNAS; i++){
        if(!SET_BLOCK(f, cchar(&D[i]), D[i-1].NEXT)){
            cout("Ocorreu um erro ao tente inserir o Meta Dado");
            return false;
        }
    }
    return true;
}

bool SET_N_TABLES(fstream *f, DATABASE *db){
    SEGMENT S;
    S.BASE = 1;
    S.LIMIT = sizeof(int);
    if(!SET_BLOCK(f, cchar(&db->BASE_HEADER.NUMERO_TABELAS), S)) return false;
    return true;
}

bool SET_REGISTER(fstream *f, REGISTRO  *R, SEGMENT P){
    if(VALID_SEG(P)) SET_BLOCK(f, cchar(R), P);
    else return false;
    return true;
}

bool SET_DADOS(fstream *f, TABLE_HEADER *TH, DATA DADOS[]){
    for(int i=1; i<TH->NUMERO_COLUNAS; i++) if(!SET_BLOCK(f, cchar(&DADOS[i]), DADOS[i-1].NEXT)) return false;
    return true;

}

bool SET_DADO(fstream *f, DATABASE *db, DATA *D, vector<string> *s, int coluna, void *p, SEGMENT POS){
    for(int i=1; i<coluna; i++){
        POS = D->NEXT;
        if(VALID_SEG(D->NEXT)){
            if(!LER_DADO(f, D, D->NEXT)) return false;
        }
        else{
            cout("Erro: proximo dado nao foi encontrado");
            return false;
        }
    }
    if((*s)[0][coluna-1] == STRING){
        if((int)(cstring(p))->size() != D->DATA_POS.LIMIT){
            if(!DDESALOCAR(f, db, D->DATA_POS)){
                cout("Erro ao inserir dado");
                return false;
            }
            D->DATA_POS = Dallocar(f, (cstring(p))->size());
            if(!SET_BLOCK(f, cchar(D), POS)){
                cout("Erro ao inserir dado");
                return false;
            }
        }
        if(!SET_BLOCK(f, cchar((cstring(p))->c_str()), D->DATA_POS)){
            cout("Erro ao inserir dado");
            return false;
        }
    }
    else{
        if(!VALID_SEG(D->DATA_POS)){
            D->DATA_POS = Dallocar(f, TSize((*s)[0][coluna-1]));
            if(!SET_BLOCK(f, cchar(D), POS)){
                cout("Erro ao inserir dado");
                return false;
            }
        }
        if(!SET_BLOCK(f, cchar(p), D->DATA_POS)){
            cout("Erro ao inserir dado");
            return false;
        }
    }
    return true;
}

bool SET_COLUNA(fstream *f, DATABASE *db, META_DADO *M, TABLE *T, string tipos, string nome){
    DATA D;
    SEGMENT S, SEG = M->TIPOS;
    if(!DDESALOCAR(f, db, SEG)){
        cout("Occoreu um erro a inserir a coluna");
        return false;
    }
    SEG = T->META_TABLE_POSITION;
    SEG.BASE += sizeof(SEGMENT);
    SEG.LIMIT = sizeof(SEGMENT);
    M->TIPOS = Dallocar(f, tipos.size());
    if(!SET_BLOCK(f, cchar(&M->TIPOS), SEG)){
        cout("Occoreu um erro a inserir a coluna");
        return false;
    }
    if(!SET_BLOCK(f, cchar(tipos.c_str()), M->TIPOS)){
        cout("Occoreu um erro a inserir a coluna");
        return false;
    }
    S.BASE = SEG.BASE+sizeof(SEGMENT);
    S.LIMIT = sizeof(DATA);
    D = M->NOMES;
    while(VALID_SEG(D.NEXT)){
        S = D.NEXT;
        if(!GET_BLOCK(f, cchar(&D), D.NEXT)){
            cout("Occoreu um erro a inserir a coluna");
            return false;
        }
    }
    if(VALID_SEG(M->NOMES.DATA_POS)){
        D.NEXT = Dallocar(f, sizeof(DATA));
        if(!SET_BLOCK(f, cchar(&D), S)){
            cout("Occoreu um erro a inserir a coluna");
            return false;
        }
    }
    DATA ax;
    ax.DATA_POS = Dallocar(f, nome.size());
    ax.NEXT.BASE = ax.NEXT.LIMIT = -1;
    if(!VALID_SEG(M->NOMES.DATA_POS)){
        if(!SET_BLOCK(f, cchar(&ax), S)){
            cout("Occoreu um erro a inserir a coluna");
            return false;
        }
    }
    else{
        if(!SET_BLOCK(f, cchar(&ax), D.NEXT)){
            cout("Occoreu um erro a inserir a coluna");
            return false;
        }
    }
    if(!SET_BLOCK(f, cchar(nome.c_str()), ax.DATA_POS)){
        cout("Occoreu um erro a inserir a coluna");
        return false;
    }
    return true;
}

bool SET_INDEX(fstream *f, TABLE *T, TABLE_HEADER *TH, REGISTRO *R, SEGMENT S){
    indice i;
    cout(T->ID_ATUAL);
    i.n = T->ID_ATUAL++;
    i.seek = S;
    SEGMENT ax;
    ax.BASE = T->INDICES.BASE + T->INDICES.LIMIT;
    ax.LIMIT = sizeof(indice);
    R->INDEX = ax;
    if(!SET_BLOCK(f, cchar(&i), ax)) return false;
    T->INDICES.LIMIT += sizeof(indice);
    if(!SET_TABLE(f, T, TH->TABLE_POSITION)) return false;
    return true;
}

void *Malocar(char TIPO){
    if(TIPO == INT) return new int;
    else if(TIPO == FLOAT) return new float;
    else if(TIPO == DOUBLE) return new double;
    else if(TIPO == CHAR) return new char;
    else if(TIPO == STRING) return new string;
    else return nullptr;
}

bool Atribuir(node *n, string s, char TIPO){
    n->info = Malocar(TIPO);
    if(n->info == nullptr){
        cout("Erro na atribuicao");
        return false;
    }
    if(TIPO == INT) *cint(n->info) = stoi(s);
    else if(TIPO == FLOAT) *cfloat(n->info) = stof(s);
    else if(TIPO == DOUBLE) *cdouble(n->info) = stod(s);
    else if(TIPO == CHAR) *cchar(n->info) = s[0];
    else if(TIPO == STRING) *cstring(n->info) = s;
    else return false;
    return true;
}

void *STRING_TO_VOID(string *s, char TIPO){
    void *p;
    if(TIPO == INT) {
        p = new int;
        *cint(p) = stoi(*s);
        return p;
    }
    else if(TIPO == FLOAT){
        p = new float;
        *cfloat(p) = stof(*s);
        return p;
    }
    else if(TIPO == DOUBLE){
        p = new double;
        *cdouble(p) = stod(*s);
        return p;
    }
    else if(TIPO == CHAR){
        p = new char;
        *cchar(p) = (*s)[0];
        return p;
    }
    else if(TIPO == STRING) return (void *) s;
    cout("Tipo informado errado");
    return nullptr;
}

bool COMPARAR(void *p, void *p2, char TIPO){
    if(TIPO == INT) return *cint(p)==*cint(p2);
    else if(TIPO == FLOAT) return *cfloat(p)==*cfloat(p2);
    else if(TIPO == DOUBLE) return *cdouble(p)==*cdouble(p2);
    else if(TIPO == CHAR) return *cchar(p)==*cchar(p2);
    else if(TIPO == STRING) return *cstring(p)==*cstring(p2);
    cout("Tipo informado errado");
    return false;
}

bool COMPARAR_VOID_STRING(void *p, string s, char TIPO){
    if(TIPO == INT) return *cint(p)==stoi(s);
    else if(TIPO == FLOAT) return *cfloat(p)==stof(s);
    else if(TIPO == DOUBLE) return *cdouble(p)==stod(s);
    else if(TIPO == CHAR) return *cchar(p)==s[0];
    else if(TIPO == STRING) return *cstring(p)==s;
    cout("Tipo informado errado");
    return false;
}

void IMPRIMIR_MR(vector<MEM_REGISTER> *v, string md){
    for(unsigned int i=0; i<(*v).size(); i++){
        cout("\n");
        for(unsigned int j=0; j<md.size(); j++){
            if(md[j] == INT){
                cout((*v)[i].vars[j].nome<<" : "<< ((*v)[i].vars[j].info ? to_string(*cint((*v)[i].vars[j].info)) : ""));
            }
            else if(md[j] == FLOAT) {
                cout((*v)[i].vars[j].nome<<" : "<< ((*v)[i].vars[j].info ? to_string(*cfloat((*v)[i].vars[j].info)) : ""));
            }
            else if(md[j] == DOUBLE) {
                cout((*v)[i].vars[j].nome<<" : "<< ((*v)[i].vars[j].info ? to_string(*cdouble((*v)[i].vars[j].info)) : ""));
            }
            else if(md[j] == CHAR) {
                cout((*v)[i].vars[j].nome<<" : "<< ((*v)[i].vars[j].info ? to_string(*cchar((*v)[i].vars[j].info)) : ""));
            }
            else if(md[j] == STRING) {
                cout((*v)[i].vars[j].nome<<" : "<< ((*v)[i].vars[j].info ? *cstring((*v)[i].vars[j].info) : ""));
            }
        }
        cout("\n");
    }
}

void FREE_MR(vector<MEM_REGISTER> *v, string md){
    for(unsigned int i=0; i<v->size(); i++)
        for(unsigned int j=0; j<(*v)[i].vars.size(); j++){
            if(md[j] == INT) delete cint((*v)[i].vars[j].info);
            else if(md[j] == FLOAT) delete cfloat((*v)[i].vars[j].info);
            else if(md[j] == DOUBLE) delete cdouble((*v)[i].vars[j].info);
            else if(md[j] == CHAR) delete cchar((*v)[i].vars[j].info);
            else if(md[j] == STRING) delete cstring((*v)[i].vars[j].info);
        }
}
//=======================================================================================================

bool CONTROL(DATABASE *db, vector<string>CONTR[]){
    /*
        CONTR, um vetor de vector de strings vc deve tá se perguntando que desgraça é essa? vem comigo que eu explico
        os vetores CONTR cada posção dele armazena informação de uma tabela ou seja cada vector de string parra informação
        sobre uma tabela.
        A primeira posição do vector de string CONTR[i][0] armazena o nome da tabela, a segunda posição CONTR[i][1] armazena
        o numero de "colunas" que essa tabela possuirá, a terceira posição armazena uma string que é o meta dado que dis o tipo
        de cada coluna indicado por um caracter, e as posições restantes armazenam o nome das colunas.
    */
    fstream DBASE;
    int NT = db->BASE_HEADER.NUMERO_TABELAS;
    TABLE_HEADER TH[NT];
    TABLE TABELAS[NT];
    META_DADO M_DADOS[NT];
    if(!ABRIR_ARQUIVO(&DBASE, db->NOME, SAIDA)) return false;
    if(!SET_DB_HEADER(&DBASE, db, false)) return false;
    if(!FILL_TABLE_HEADER(&DBASE, &TH[0], stoi(CONTR[0][1]))) return false;
    if(!SET_TABLE_HEADER(&DBASE, &TH[0], db->TABELAS_HEADER_SEG)) return false;

    for(int i=1;i<NT-1;i++)
        if(!(FILL_TABLE_HEADER(&DBASE, &TH[i], stoi(CONTR[i][1]))&&SET_TABLE_HEADER(&DBASE, &TH[i], TH[i-1].NEXT))) return false;

    if(!FILL_NEW_TABLE_HEADER(&DBASE, &TH[NT-1], stoi(CONTR[NT-1][1]))) return false;
    if(!SET_TABLE_HEADER(&DBASE, &TH[NT-1], TH[NT-2].NEXT)) return false;

    for(int i=0; i<NT; i++)
        if(!(FILL_TABLE(&DBASE, &TABELAS[i], &TH[i]) && SET_TABLE(&DBASE, &TABELAS[i], TH[i].TABLE_POSITION))) return false;

    for(int i=0; i<NT; i++){
        DATA D[TH[i].NUMERO_COLUNAS];
        if(!(FILL_MDADO(&DBASE, &M_DADOS[i], &TH[i], &CONTR[i], D) && SET_MDADO(&DBASE, &M_DADOS[i], &TH[i], &TABELAS[i], D))) return false;
    }
    DBASE.close();
    return true;
}

bool VALID_SEG(SEGMENT S){
    return(S.BASE!=-1 && S.LIMIT!=-1);
}

SEGMENT SEARCH_TABLE(fstream *f, DATABASE *db, TABLE_HEADER *TH, TABLE *T, META_DADO *M, string nome){
    SEGMENT S, ax;
    S.BASE = S.LIMIT = -1;
    ax = db->TABELAS_HEADER_SEG;
    if(!LER_TABLE_HEADER(f, TH, db->TABELAS_HEADER_SEG)) return S;
    if(!LER_TABELA(f, T, TH->TABLE_POSITION)) return S;
    for(int i=0; i<db->BASE_HEADER.NUMERO_TABELAS; i++){
        if(!LER_MDADO(f, M, T)) return S;
        string s;
        s.resize(M->NOME.LIMIT);
        if(!GET_BLOCK(f, cchar(s.c_str()), M->NOME)){
            return S;
        }
        if(s == nome) {
            return ax;
        }
        ax = TH->NEXT;
        if(!(LER_TABLE_HEADER(f, TH, TH->NEXT) && LER_TABELA(f, T, TH->TABLE_POSITION))){
            cout("ERRO! Tabela não encontrada");
            return S;
        }
    }
    cout("ERRO! Tabela não encontrada");
    return S;
}

string PEGAR_REG_TIPO(DATABASE *db, string tabela){
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE T;
    META_DADO META;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return "";
    if(!VALID_SEG(SEARCH_TABLE(DBASE, db, &TH, &T, &META, tabela))){
        cout("ERRO ao buscar metadados");
        DBASE->close();
        delete DBASE;
        return "";
    }
    string ax;
    ax.resize(META.TIPOS.LIMIT);
    if(!GET_BLOCK(DBASE, cchar(ax.c_str()), META.TIPOS)){
        cout("ERRO ao buscar metadados");
        DBASE->close();
        delete DBASE;
        return "";
    }
    return ax;
}

SEGMENT PEGAR_TABLE_INDEX(DATABASE *db, TABLE_HEADER *TH, TABLE *T, META_DADO *M, string tabela){
    fstream *DBASE;
    SEGMENT S;
    DBASE = OPEN_DATABASE(db);
    S.BASE = S.LIMIT = -1;
    if(DBASE == nullptr) return S;
    if(!VALID_SEG(SEARCH_TABLE(DBASE, db, TH, T, M, tabela))){
        cout("Tabela nao encontrada");
        DBASE->close();
        delete DBASE;
        return S;
    }
    DBASE->close();
    delete DBASE;
    return T->INDICES;
}

bool MDADO_TO_STRING(META_DADO *M, vector<string> *s, fstream *f){
    /*
        retorna os tipos ("s,i,s"); [0]
        e os nomes das variaveis. [1...TAM];
    */
    (*s)[0].resize(M->TIPOS.LIMIT);
    if(!GET_BLOCK(f, cchar((*s)[0].c_str()), M->TIPOS)){
        cout("Ocorreu um erro ao tentar transformar o metadado");
        return false;
    }
    DATA D = M->NOMES;
    for(unsigned int i=1; i<(*s).size(); i++){
        (*s)[i].resize(D.DATA_POS.LIMIT);
        if(!GET_BLOCK(f, cchar((*s)[i].c_str()), D.DATA_POS)){
            cout("Ocorreu um erro ao tentar transformar o metadado");
            return false;
        }
        if(VALID_SEG(D.NEXT)){
            if(!GET_BLOCK(f, cchar(&D), D.NEXT)){
                cout("Ocorreu um erro ao tentar transformar o metadado");
                return false;
            }
        }
    }
    return true;
}

bool LER_TABLE_HEADER(fstream *f, TABLE_HEADER *TH, SEGMENT POS){
    if(!GET_BLOCK(f, cchar(TH), POS)){
        cout("Tabela inesistente");
        return false;
    }
    return true;
}

bool LER_TABELA(fstream *f, TABLE *T, SEGMENT POS){
    if(!GET_BLOCK(f, cchar(T), POS)){
        cout("Ocorreu um erro ao buscar a tabela");
        return false;
    }
    return true;
}

bool LER_MDADO(fstream *f, META_DADO *M, TABLE *T){
    if(!GET_BLOCK(f, cchar(M), T->META_TABLE_POSITION)){
        cout("Ocorreu um erro ao pegar metadado");
        return false;
    }
    return true;
}

bool LER_REGISTRO(fstream *f, REGISTRO *R, SEGMENT POS){
    if(!GET_BLOCK(f, cchar(R), POS)){
        cout("Ocorreu um erro ao ler o registro");
        return false;
    }
    return true;
}

bool LER_INDEX(fstream *f, indice *i, REGISTRO *R){
    if(!GET_BLOCK(f, cchar(i), R->INDEX)){
        cout("Ocorreu um erro ao ler o index");
        return false;
    }
    return true;
}

bool LER_DADO(fstream *f, DATA *D, SEGMENT POS){
    if(!GET_BLOCK(f, cchar(D), POS)){
        cout("Erro ao ler o dado");
        return false;
    }
    return true;
}

bool RtoMR(fstream *f, vector<string> *s, REGISTRO *R, MEM_REGISTER *MR){
    DATA D = R->DADOS;
    for(unsigned int i=0; i<(*s)[0].size(); i++){
        node n;
        n.nome = (*s)[i+1];
        n.info = nullptr;
        if(VALID_SEG(D.DATA_POS)){
            n.info = Malocar((*s)[0][i]);
            if((*s)[0][i] == STRING){
                (*cstring(n.info)).resize(D.DATA_POS.LIMIT);
                if(!GET_BLOCK(f, cchar((*cstring(n.info)).c_str()), D.DATA_POS)){
                    cout("Ocorreu um erro ao transformar o registro");
                    return false;
                }
            }
            else if(!GET_BLOCK(f, cchar(n.info), D.DATA_POS)){
                cout("Ocorreu um erro ao transformar o registro");
                return false;
            }
        }
        MR->vars.push_back(n);
        if(VALID_SEG(D.NEXT)){
            if(!GET_BLOCK(f, cchar(&D), D.NEXT)){
                cout("Ocorreu um erro ao transformar o registro");
                return false;
            }
        }
    }
    return true;
}

//=======================================================================================================

bool CRIAR_DATABASE(string NOME, vector<string>CONTR[], int N_TAB){
    DATABASE db;
    db.NOME = NOME;
    db.BASE_HEADER.NUMERO_TABELAS = N_TAB;
    db.SEGMENTOS_LIVRES.BASE = db.SEGMENTOS_LIVRES.LIMIT = -1;
    return CONTROL(&db, CONTR);
}

bool CRIAR_DATABASE_VAZIO(string nome){
    DATABASE db;
    db.NOME = nome;
    db.TABELAS_HEADER_SEG.BASE = db.TABELAS_HEADER_SEG.LIMIT = -1;
    db.SEGMENTOS_LIVRES.BASE = db.SEGMENTOS_LIVRES.LIMIT = -1;
    db.BASE_HEADER.NUMERO_TABELAS = 0;
    fstream DBASE;
    if(!ABRIR_ARQUIVO(&DBASE, db.NOME, SAIDA)){
        cout("Erro ao criar banco");
        return false;
    }
    if(!SET_DB_HEADER(&DBASE, &db, true)){
        cout("Erro ao criar banco");
        return false;
    }
    return true;
}

bool INSERIR_TABELA(DATABASE *db, vector<string> *CONTR){
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE  T;
    META_DADO M;
    SEGMENT SEG;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return false;
    SEG = db->TABELAS_HEADER_SEG;
    if(!LER_TABLE_HEADER(DBASE, &TH, db->TABELAS_HEADER_SEG)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    while(VALID_SEG(TH.NEXT)){
        SEG = TH.NEXT;
        if(!LER_TABLE_HEADER(DBASE, &TH, TH.NEXT)){
            DBASE->close();
            delete DBASE;
            return false;
        }
    }
    SEG.BASE += SEG.LIMIT-sizeof(SEGMENT);
    TH.NEXT = Dallocar(DBASE, sizeof(TABLE_HEADER));
    if(!SET_BLOCK(DBASE, cchar(&TH.NEXT), SEG)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    SEG = TH.NEXT;
    if(!(FILL_NEW_TABLE_HEADER(DBASE, &TH, stoi((*CONTR)[1])) && SET_TABLE_HEADER(DBASE, &TH, SEG))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!(FILL_TABLE(DBASE, &T, &TH)&&SET_TABLE(DBASE, &T, TH.TABLE_POSITION))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    DATA D[TH.NUMERO_COLUNAS];
    if(!(FILL_MDADO(DBASE, &M, &TH, CONTR, D) && SET_MDADO(DBASE, &M, &TH, &T, D))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    db->BASE_HEADER.NUMERO_TABELAS++;
    if(!SET_N_TABLES(DBASE, db)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    DBASE->close();
    delete DBASE;
    return true;
}

bool INSERIR_TABELA_VAZIA(DATABASE *db, string nome){
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE  T;
    META_DADO M;
    SEGMENT SEG;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return false;
    SEG = db->TABELAS_HEADER_SEG;
    cout(SEG.BASE);
    if(!VALID_SEG(SEG)){
        TH.NEXT.BASE = TH.NEXT.LIMIT = -1;
    }else{
        if(!LER_TABLE_HEADER(DBASE, &TH, db->TABELAS_HEADER_SEG)){
            DBASE->close();
            delete DBASE;
            return false;
        }
    }
    cout(SEG.BASE);
    while(VALID_SEG(TH.NEXT)){
        SEG = TH.NEXT;
        if(!LER_TABLE_HEADER(DBASE, &TH, TH.NEXT)){
            DBASE->close();
            delete DBASE;
            return false;
        }
    }
    cout(SEG.BASE);
    if(VALID_SEG(SEG)) SEG.BASE += SEG.LIMIT-sizeof(SEGMENT);
    else {
        SEG.BASE = 1 + sizeof(int);
        SEG.LIMIT = sizeof(SEGMENT);
    }
    cout(SEG.BASE);
    TH.NEXT = Dallocar(DBASE, sizeof(TABLE_HEADER));
    if(!SET_BLOCK(DBASE, cchar(&TH.NEXT), SEG)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    SEG = TH.NEXT;
    if(!(FILL_NEW_TABLE_HEADER(DBASE, &TH, 0) && SET_TABLE_HEADER(DBASE, &TH, SEG))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!(FILL_TABLE(DBASE, &T, &TH)&&SET_TABLE(DBASE, &T, TH.TABLE_POSITION))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    M.NOME = Dallocar(DBASE, nome.size());
    if(!SET_BLOCK(DBASE, cchar(nome.c_str()), M.NOME)){
        cout("ERRO na criacao da tabela");
        DBASE->close();
        delete DBASE;
        return false;
    }
    M.TIPOS.BASE = M.TIPOS.LIMIT = -1;
    M.NOMES.NEXT = M.TIPOS;
    M.NOMES.DATA_POS = M.NOMES.NEXT;
    if(!SET_BLOCK(DBASE, cchar(&M), T.META_TABLE_POSITION)){
        cout("ERRO na criacao da tabela");
        DBASE->close();
        delete DBASE;
        return false;
    }
    db->BASE_HEADER.NUMERO_TABELAS++;
    if(!SET_N_TABLES(DBASE, db)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    DBASE->close();
    delete DBASE;
    return true;
}

bool INSERIR_COLUNA(DATABASE *db, string NomeTabela, string NomeColuna, char tipo){
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE T;
    META_DADO M;
    SEGMENT S;
    REGISTRO R;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return false;
    S = SEARCH_TABLE(DBASE, db, &TH, &T, &M, NomeTabela);
    if(!VALID_SEG(S)){
        cout("Ocorreu um erro ao inserir coluna");
        DBASE->close();
        delete DBASE;
        return false;
    }
    TH.NUMERO_COLUNAS++;
    if(!SET_TABLE_HEADER(DBASE, &TH, S)){
        cout("Ocorreu um erro ao inserir coluna");
        DBASE->close();
        delete DBASE;
        return false;
    }
    string ax;
    if(VALID_SEG(M.TIPOS)){
        ax.resize(M.TIPOS.LIMIT);
        if(!GET_BLOCK(DBASE, cchar(ax.c_str()), M.TIPOS)){
            cout("Ocorreu um erro ao inserir coluna");
            DBASE->close();
            delete DBASE;
            return false;
        }
    }
    ax.push_back(tipo);
    if(!SET_COLUNA(DBASE, db, &M, &T, ax, NomeColuna)){
        cout("Ocorreu um erro ao inserir coluna");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(VALID_SEG(T.REGISTROS)){
        if(!LER_REGISTRO(DBASE, &R, T.REGISTROS)){
            cout("Ocorreu um erro ao inserir coluna");
            DBASE->close();
            delete DBASE;
            return false;
        }
        SEGMENT S = T.REGISTROS;
        S.LIMIT = sizeof(DATA);
        for(int i=0; i<TH.NUMERO_REGISTROS; i++){
            DATA D = R.DADOS;
            while(VALID_SEG(D.NEXT)){
                S = D.NEXT;
                if(!GET_BLOCK(DBASE, cchar(&D), D.NEXT)){
                    cout("Ocorreu um erro ao inserir coluna");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
            }
            D.NEXT = Dallocar(DBASE, sizeof(DATA));
            if(!SET_BLOCK(DBASE, cchar(&D), S));
            DATA ax;
            ax.DATA_POS.BASE = ax.DATA_POS.LIMIT = -1;
            ax.NEXT = ax.DATA_POS;
            if(!SET_BLOCK(DBASE, cchar(&ax), D.NEXT)){
                cout("Ocorreu um erro ao inserir coluna");
                DBASE->close();
                delete DBASE;
                return false;
            }
            S = R.NEXT;
            S.LIMIT = sizeof(DATA);
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, R.NEXT)){
                    cout("Ocorreu um erro ao inserir coluna");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
            }else break;
        }
    }
    DBASE->close();
    delete DBASE;
    return true;
}

bool INSERIR_REGISTRO(DATABASE *db, MEM_REGISTER REGISTER){
    fstream *DBASE;
    SEGMENT SH, P;
    TABLE_HEADER TH;
    TABLE TABELA;
    META_DADO MDADO;
    REGISTRO R;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return false;
    SH = SEARCH_TABLE(DBASE, db, &TH, &TABELA, &MDADO, REGISTER.nome);
    if(!VALID_SEG(SH)){
        DBASE->close();
        delete DBASE;
        cout("Ocorreu um erro ao inserir o registro");
        return false;
    }
    if(TH.NUMERO_COLUNAS == 0){
        DBASE->close();
        delete DBASE;
        cout("A tabela nao possui colunas");
        return false;
    }
    SEGMENT SEG;
    if(VALID_SEG(TABELA.REGISTROS)){
        P = TABELA.REGISTROS;
        cout("TESTE 1");
        if(!LER_REGISTRO(DBASE, &R, TABELA.REGISTROS)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao inserir o registro");
            return false;
        }
        cout(TH.NUMERO_REGISTROS);
        for(int i=0; i<TH.NUMERO_REGISTROS-1; i++){
            P = R.NEXT;
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, R.NEXT)){
                    DBASE->close();
                    delete DBASE;
                    cout("Ocorreu um erro ao inserir o registro");
                    return false;
                }
            }
        }
        cout("TESTE 1");
        SEG = Dallocar(DBASE, sizeof(REGISTRO));
        R.NEXT = SEG;
        if(!SET_REGISTER(DBASE, &R, P)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao inserir registro");
            return false;
        }
        //DBASE->seekg((P.BASE+P.LIMIT)-sizeof(SEGMENT));
        //DBASE->write(cchar(&R.NEXT), sizeof(SEGMENT));
        cout("TESTE 1");
        TH.NUMERO_REGISTROS++;
        if(!SET_TABLE_HEADER(DBASE, &TH, SH)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao inserir registro");
            return false;
        }
        if(!SET_INDEX(DBASE, &TABELA, &TH, &R, SEG)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao criar indice");
            return false;
        }
        cout("TESTE 1");
    }
    else{
            cout("TESTE 2");
        TH.NUMERO_REGISTROS++;
        if(!SET_TABLE_HEADER(DBASE, &TH, SH)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao inserir registro");
            return false;
        }
        SEG = Dallocar(DBASE, sizeof(REGISTRO));
        TABELA.REGISTROS = SEG;
        if(!SET_TABLE(DBASE, &TABELA, TH.TABLE_POSITION)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao inserir registro");
            return false;
        }
        if(!SET_INDEX(DBASE, &TABELA, &TH, &R,SEG)){
            DBASE->close();
            delete DBASE;
            cout("Ocorreu um erro ao criar indice");
            return false;
        }
    }
    DATA DADOS[TH.NUMERO_COLUNAS];
    if(!FILL_DADOS(DBASE, &TH, DADOS)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&MDADO, &s, DBASE)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(REGISTER.vars[0].nome.empty())
        for(unsigned int i=1; i<s.size(); i++){
            REGISTER.vars[i-1].nome = s[i];
        }
    for(unsigned int i=0; i<REGISTER.vars.size(); i++){
        for(int j=0; j<TH.NUMERO_COLUNAS; j++){
            if(s[j+1] == REGISTER.vars[i].nome){
                if(REGISTER.vars[i].info != nullptr){
                    DADOS[j].DATA_POS = Dallocar(DBASE, TSize(s[0][j]));
                    if(!VALID_SEG(DADOS[j].DATA_POS)) DADOS[j].DATA_POS = Dallocar(DBASE, (*cstring(REGISTER.vars[i].info)).size());
                    if(s[0][j]==STRING) SET_BLOCK(DBASE, cchar((*cstring(REGISTER.vars[j].info)).c_str()), DADOS[j].DATA_POS);
                    else SET_BLOCK(DBASE, cchar(REGISTER.vars[j].info), DADOS[j].DATA_POS);
                    }
                else{
                    DADOS[j].DATA_POS.BASE = DADOS[j].DATA_POS.LIMIT = -1;
                }
            }
        }
    }
    R.DADOS = DADOS[0];
    R.NEXT.BASE = R.NEXT.LIMIT = -1;
    if(!(SET_REGISTER(DBASE, &R, SEG) && SET_DADOS(DBASE, &TH, DADOS))){
        DBASE->close();
        delete DBASE;
        return false;
    }
    DBASE->close();
    delete DBASE;
    return true;
}

bool UPDATE_REGISTRO(DATABASE *db, string CONTROLE[4], string valor){
    /*
        CONTROLE[0] -> Nome da tabela
        Se CONTROLE[1] for "ID" então devo atualizar o registro com o ID == CONTROLE[2]
        Caso não, será o nome da variavel que tem que ser igual a p2
        e CONTROLE[3] é a coluna que deve ser atualizada igual a p
    */
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE T;
    META_DADO M;
    REGISTRO R;
    int coluna = 0;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return false;
    SEGMENT S = SEARCH_TABLE(DBASE, db, &TH, &T, &M, CONTROLE[0]);
    if(!VALID_SEG(S)){
        cout("ERRO Tabela nao encontrada");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!VALID_SEG(T.REGISTROS)){
        cout("A tabela nao possui registros");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!LER_REGISTRO(DBASE, &R, T.REGISTROS)){
        cout("ERRO ao ler registro");
        DBASE->close();
        delete DBASE;
        return false;
    }
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&M, &s, DBASE)){
        cout("ERRO ao transformar metadado");
        DBASE->close();
        delete DBASE;
        return false;
    }
    for(int i=1; i<TH.NUMERO_COLUNAS+1; i++){
        if(CONTROLE[3] == s[i]) {
            coluna = i;
            break;
        }
    }
    if(coluna == 0){
        cout("ERRO: Coluna nao existe");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(CONTROLE[1]=="ID"){
        int id = stoi(CONTROLE[2]);
        SEGMENT POS;
        DATA D;
        indice ind;
        POS = T.REGISTROS;
        POS.LIMIT = sizeof(DATA);
        do{
            if(!LER_INDEX(DBASE, &ind, &R)){
                DBASE->close();
                delete DBASE;
                return false;
            }
            if(ind.n == id){
                D = R.DADOS;
                if(!SET_DADO(DBASE, db, &D, &s, coluna, STRING_TO_VOID(&valor, s[0][coluna-1]), POS)){
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
                break;
            }
            POS = R.NEXT;
            POS.LIMIT = sizeof(DATA);
        }while(VALID_SEG(R.NEXT) && LER_REGISTRO(DBASE, &R, R.NEXT));
    }
    else if(!CONTROLE[2].empty()){
        int coln = 0;
        MEM_REGISTER mr;
        SEGMENT POS;
        DATA D;
        POS = T.REGISTROS;
        POS.LIMIT = sizeof(DATA);
        for(int i=1; i<TH.NUMERO_COLUNAS+1; i++){
            if(CONTROLE[1] == s[i]) {
                coln = i;
                break;
            }
        }
        if(coln == 0){
                cout("ERRO: Coluna nao existe");
                DBASE->close();
                delete DBASE;
                return false;
        }
        do{
            if(!RtoMR(DBASE, &s, &R, &mr)){
                DBASE->close();
                delete DBASE;
                return false;
            }
            if(COMPARAR_VOID_STRING(mr.vars[coln-1].info, CONTROLE[2], s[0][coln-1])){
                D = R.DADOS;
                if(!SET_DADO(DBASE, db, &D, &s, coluna, STRING_TO_VOID(&CONTROLE[2], s[0][coluna-1]), POS)){
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
            }
            POS = R.NEXT;
            POS.LIMIT = sizeof(DATA);
            mr.vars.clear();
        }while(VALID_SEG(R.NEXT) && LER_REGISTRO(DBASE, &R, R.NEXT));
    }
    else{
        SEGMENT POS;
        DATA D;
        void *p = STRING_TO_VOID(&valor, s[0][coluna-1]);
        POS = T.REGISTROS;
        POS.LIMIT = sizeof(DATA);
        do{
            D = R.DADOS;
            if(!SET_DADO(DBASE, db, &D, &s, coluna, p, POS)){
                DBASE->close();
                delete DBASE;
                return false;
            }
            POS = R.NEXT;
            POS.LIMIT = sizeof(DATA);
        }while(VALID_SEG(R.NEXT) && LER_REGISTRO(DBASE, &R, R.NEXT));
    }
    DBASE->close();
    delete DBASE;
    return true;
}

bool DELETAR_REGISTRO(DATABASE *db, string CONTROLE[3], void **p){
    /*
        CONTROLE[0] -> Nome da tabela
        Se CONTROLE[1] for "ID" então devo deletar o CONTROLE[2] ésimo registro
        Caso não, será o nome da variavel que tem que ser igual a *p
        Se CONTROLE[1] for "*" Deve deletar todos os registros
    */
    fstream *DBASE;
    TABLE_HEADER TH;
    TABLE T;
    META_DADO M;
    REGISTRO R;
    DBASE = OPEN_DATABASE(db);
    SEGMENT S;
    if(DBASE == nullptr) return false;
    S = SEARCH_TABLE(DBASE, db, &TH, &T, &M, CONTROLE[0]);
    if(!VALID_SEG(S)){
        cout("ERRO Tabela nao encontrada");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!VALID_SEG(T.REGISTROS)){
        cout("A tabela nao possui registros");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(!LER_REGISTRO(DBASE, &R, T.REGISTROS)){
        cout("ERRO ao ler registro");
        DBASE->close();
        delete DBASE;
        return false;
    }
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&M, &s, DBASE)){
        cout("ERRO ao transformar metadado");
        DBASE->close();
        delete DBASE;
        return false;
    }
    if(CONTROLE[1] == "ID"){
        int id = stoi(CONTROLE[2]);
        indice ind;
        SEGMENT S2 = T.REGISTROS;
        SEGMENT ax;
        if(!LER_REGISTRO(DBASE, &R, T.REGISTROS)){
            cout("ERRO ao ler registro");
            DBASE->close();
            delete DBASE;
            return false;
        }
        do{
            if(!GET_BLOCK(DBASE, cchar(&ind), R.INDEX)){
                cout("ERRO");
                DBASE->close();
                delete DBASE;
                return false;
            }
            if(ind.n == id){
                if(!DELETAR_DADOS(DBASE, db, &R.DADOS)){
                    cout("ERRO");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
                if(!DDESALOCAR(DBASE, db, S2)){
                    cout("ERRO");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
                TH.NUMERO_REGISTROS--;
                if(!SET_TABLE_HEADER(DBASE, &TH, S)){
                    cout("ERRO");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
                if(ind.seek.BASE == T.REGISTROS.BASE){
                    T.REGISTROS = R.NEXT;
                    if(!SET_TABLE(DBASE, &T, TH.TABLE_POSITION)){
                        cout("ERRO");
                        DBASE->close();
                        delete DBASE;
                        return false;
                    }
                }
                else{
                    if(!SET_BLOCK(DBASE, cchar(&R.NEXT), ax)){
                        cout("ERRO");
                        DBASE->close();
                        delete DBASE;
                        return false;
                    }
                }
                ind.seek.BASE = ind.seek.LIMIT = -1;
                if(!SET_BLOCK(DBASE, cchar(&ind), R.INDEX)){
                    cout("ERRO");
                    DBASE->close();
                    delete DBASE;
                    return false;
                }
                DBASE->close();
                delete DBASE;
                return true;
            }
            ax = S2;
            ax.BASE += ax.LIMIT-sizeof(SEGMENT);
            ax.LIMIT = sizeof(SEGMENT);
            S2 = R.NEXT;
        }while(VALID_SEG(R.NEXT) && LER_REGISTRO(DBASE, &R, R.NEXT));
    }
    else if(CONTROLE[1] == "*"){
        SEGMENT S2 = T.REGISTROS;
        if(!LER_REGISTRO(DBASE, &R, T.REGISTROS)){
            cout("ERRO");
            DBASE->close();
            delete DBASE;
            return false;
        }
        do{
            indice ind;
            if(!GET_BLOCK(DBASE, cchar(&ind), R.INDEX)){
                cout("ERRO");
                DBASE->close();
                delete DBASE;
                return false;
            }
            if(!DELETAR_DADOS(DBASE, db, &R.DADOS)){
                cout("ERRO");
                DBASE->close();
                delete DBASE;
                return false;
            }
            ind.seek.BASE = ind.seek.LIMIT = -1;
            if(!SET_BLOCK(DBASE, cchar(&ind), R.INDEX)){
                cout("ERRO");
                DBASE->close();
                delete DBASE;
                return false;
            }
            if(!DDESALOCAR(DBASE, db, S2)){
                cout("ERRO");
                DBASE->close();
                delete DBASE;
                return false;
            }
            S2 = R.NEXT;
        }while(VALID_SEG(R.NEXT) && LER_REGISTRO(DBASE, &R, R.NEXT));
        T.INDEX_AREA.LIMIT = 0;
        T.REGISTROS.BASE = T.REGISTROS.LIMIT = -1;
        TH.NUMERO_REGISTROS = 0;
        if(!(SET_TABLE(DBASE, &T, TH.TABLE_POSITION) && SET_TABLE_HEADER(DBASE, &TH, S)));
        DBASE->close();
        delete DBASE;
        return true;
    }
    else if(*p!=nullptr){

    }
    DBASE->close();
    delete DBASE;
    return false;
}

bool DELETAR_DADOS(fstream *f, DATABASE *db, DATA *D){
    if(VALID_SEG(D->DATA_POS)) if(!DDESALOCAR(f, db, D->DATA_POS)) return false;
    while(VALID_SEG(D->NEXT)){
        if(!DDESALOCAR(f, db, D->NEXT)) return false;
        if(!GET_BLOCK(f, cchar(D), D->NEXT));
        if(VALID_SEG(D->DATA_POS)) if(!DDESALOCAR(f, db, D->DATA_POS)) return false;
    }
    if(VALID_SEG(D->DATA_POS)) if(!DDESALOCAR(f, db, D->DATA_POS)) return false;
    return true;
}

vector<MEM_REGISTER> PEGAR_REGISTRO(DATABASE *db, vector<string> *CONTROLE){
    /*
        CONTROLE[0] -> Nome da tabela
        Se CONTROLE[1] for "ID" então devo retornar o CONTROLE[2] ésimo registro
        Caso não, será o nome da variavel que tem que ser igual a CONTROLE[2]
        Se CONTROLE[1] for "" Deve retornar todos os registros
    */
    fstream *DBASE;
    REGISTRO R;
    vector<MEM_REGISTER> MRL;
    TABLE_HEADER TH;
    TABLE TABELA;
    META_DADO MDADO;
    DBASE = OPEN_DATABASE(db);
    if(DBASE == nullptr) return MRL;
    if(!LER_TABLE_HEADER(DBASE, &TH, db->TABELAS_HEADER_SEG)){
        DBASE->close();
        delete DBASE;
        return MRL;
    }
    if(!LER_TABELA(DBASE, &TABELA, TH.TABLE_POSITION)){
        DBASE->close();
        delete DBASE;
        return MRL;
    }
    while(true){
        if(!LER_MDADO(DBASE, &MDADO, &TABELA)){
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        string ax;
        ax.resize(MDADO.NOME.LIMIT);
        if(!GET_BLOCK(DBASE, cchar(ax.c_str()), MDADO.NOME)){
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        if(ax == (*CONTROLE)[0]) break;
        if(!VALID_SEG(TH.NEXT)){
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        if(!(LER_TABLE_HEADER(DBASE, &TH, TH.NEXT) && LER_TABELA(DBASE, &TABELA, TH.TABLE_POSITION))){
            DBASE->close();
            delete DBASE;
            return MRL;
        }
    }
    if(TH.NUMERO_REGISTROS == 0){
        cout("A tabela nao possui registros");
        DBASE->close();
        delete DBASE;
        return MRL;
    }
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&MDADO, &s, DBASE)){
        DBASE->close();
        delete DBASE;
        return MRL;
    }
    MEM_REGISTER MR;
    if((*CONTROLE)[1]=="ID"){
        indice ind;
        SEGMENT SEG;
        int  p;
        ind.n = stoi((*CONTROLE)[2]);
        BTree ax = criarNo();
        carregaBTree(cchar(db->NOME.c_str()), &ax, TABELA.INDICES);
        ax = buscaChave(ax, ind.n, &p);
        if(p != -1) SEG = ax->chave[p].seek;
        else{
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        if(!LER_REGISTRO(DBASE, &R, SEG)) {
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        if(!RtoMR(DBASE, &s, &R, &MR)) {
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        MRL.push_back(MR);
    }
    else if(!(*CONTROLE)[2].empty()){
        int n;
        for(n=1; n<=TH.NUMERO_COLUNAS; n++) if(s[n]==(*CONTROLE)[1]) break;
        if(n>TH.NUMERO_COLUNAS){
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        if(!LER_REGISTRO(DBASE, &R, TABELA.REGISTROS)) {
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        for(int i=0; i<TH.NUMERO_REGISTROS; i++){
            if(!RtoMR(DBASE, &s, &R, &MR)){
                DBASE->close();
                delete DBASE;
                MRL.clear();
                return MRL;
            }
            if(COMPARAR_VOID_STRING(MR.vars[n-1].info, (*CONTROLE)[2], s[0][n-1])) MRL.push_back(MR);
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, R.NEXT)){
                    (*DBASE).close();
                    delete DBASE;
                    MRL.clear();
                    return MRL;
                }
            }
            else break;
            MR.vars.clear();
        }
    }
    else{
        if(!LER_REGISTRO(DBASE, &R, TABELA.REGISTROS)) {
            DBASE->close();
            delete DBASE;
            return MRL;
        }
        for(int i=0; i<TH.NUMERO_REGISTROS; i++){
            if(!RtoMR(DBASE, &s, &R, &MR)){
                DBASE->close();
                delete DBASE;
                MRL.clear();
                return MRL;
            }
            MRL.push_back(MR);
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, R.NEXT)){
                    DBASE->close();
                    delete DBASE;
                    MRL.clear();
                    return MRL;
                }
            }
            else break;
            MR.vars.clear();
        }
    }
    (*CONTROLE)[2] = s[0];
    DBASE->close();
    delete DBASE;
    return MRL;
}

fstream *OPEN_DATABASE(DATABASE *db){
    char BIT;
    fstream  *DBASE = new fstream;
    if(!ABRIR_ARQUIVO(DBASE, db->NOME, ENTRADA_SAIDA)){
        cout("Nao foi possivel abrir o banco");
        delete DBASE;
        return nullptr;
    }
    (*DBASE).read(cchar(&BIT), 1);
    if(BIT!=BIT_PROTETOR){
        cout("ERRO! Este arquivo nao e valido: "<<BIT<<" "<<BIT_PROTETOR);
        delete DBASE;
        return nullptr;
    }
    (*DBASE).read(cchar(&db->BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    (*DBASE).read(cchar(&db->TABELAS_HEADER_SEG), sizeof(SEGMENT));
    (*DBASE).read(cchar(&db->SEGMENTOS_LIVRES), sizeof(SEGMENT));
    return DBASE;
}

bool ABRIR_ARQUIVO(fstream *f, string NOME, char TIPO_ABERTURA){
    if(TIPO_ABERTURA == ENTRADA) (*f).open(NOME, fstream::in | fstream::binary);
    else if(TIPO_ABERTURA == SAIDA) (*f).open(NOME, fstream::out | fstream::binary);
    else if(TIPO_ABERTURA == ENTRADA_SAIDA) (*f).open(NOME, fstream::in | fstream::out | fstream::binary);
    return (*f).is_open();
}
