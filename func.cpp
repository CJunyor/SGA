#include "func.h"

void ImprimeMenu(){
    cout("================================================");
    cout("          1 - Abrir uma Tabela");
    cout("          2 - Criar Tabela");
    cout("          3 - Sair");
    cout("================================================");
    cout<<">";
}

void ImprimeSubMenu(){
    cout("================================================");
    cout("          1 - Atribuir Valores");
    cout("          2 - Mostrar Valores");
    cout("          3 - Salvar");
    cout("          4 - Voltar ao Menu");
    cout("================================================");
    cout<<">";
}

void *Mallocar(char c, void *dado){
    void *ax=nullptr;
    if(c==INT) {
        ax=new int;
        *cint(ax)=*cint(dado);
    }
    else if(c==FLOAT) {
        ax=new float;
        *cfloat(ax)=*cfloat(dado);
    }
    else if(c==DOUBLE) {
        ax=new double;
        *cdouble(ax)=*cdouble(dado);
    }
    else if(c==CHAR) {
        ax=new char;
        *cchar(ax)=*cchar(dado);
    }
    else if(c==STRING) {
        ax=new string;
        *cstring(ax)=*cstring(dado);
    }
    return ax;
}

void *Alloc(char c){
    void *ax=nullptr;
    if(c==INT) {
        ax=new int;
        cin>>*cint(ax);
    }
    else if(c==FLOAT) {
        ax=new float;
        cin>>*cfloat(ax);
    }
    else if(c==DOUBLE) {
        ax=new double;
        cin>>*cdouble(ax);
    }
    else if(c==CHAR) {
        ax=new char;
        cin>>*cchar(ax);
    }
    else if(c==STRING) {
        ax=new string;
        cin>>*cstring(ax);
    }
    return ax;
}

void Desaloc(char c, void *p){
    if(c==INT) delete cint(p);
    else if(c==CHAR) delete cchar(p);
    else if(c==FLOAT) delete cfloat(p);
    else if(c==DOUBLE) delete cdouble(p);
    else if(c==STRING) delete cstring(p);
}


//=======================================================================================================

int TSize(char TIPO){
    if(TIPO==INT) return sizeof(int);
    else if(TIPO==CHAR) return sizeof(char);
    else if(TIPO==FLOAT) return sizeof(float);
    else if(TIPO==DOUBLE) return sizeof(double);
    else return -1;
}

SEGMENT Dallocar(fstream *f, int TAM){
    //Aloca um espaço no final do arquivo
    SEGMENT S;
    if(TAM>0){
        char lixo[TAM];
        (*f).seekg(0, fstream::end);
        S.BASE=(*f).tellg();
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

bool FILL_TABLE(fstream *f, TABLE *T, TABLE_HEADER *TH){
    T->META_TABLE_POSITION = Dallocar(f, sizeof(SEGMENT)*(2+TH->NUMERO_COLUNAS));
    T->REGISTROS.BASE = T->REGISTROS.LIMIT = -1;
    return true;
}

bool FILL_MDADO(fstream *f, M_DADO *M, TABLE_HEADER *TH, vector<string> *v){
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
    M->NOMES = new SEGMENT[TH->NUMERO_COLUNAS];
    for(int i=0; i<TH->NUMERO_COLUNAS; i++){
        M->NOMES[i] = Dallocar(f, (*v)[i+3].size());
        if(!SET_BLOCK(f, cchar((*v)[i+3].c_str()), M->NOMES[i])){
            cout("Erro ao preencher Meta Dado");
            return false;
        }
    }
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
    (*f).seekg(S.BASE);
    (*f).write(p, S.LIMIT);
    return true;
}

bool SET_DB_HEADER(fstream *f, DATABASE *db){
    char BIT = BIT_PROTETOR;
    SEGMENT S;
    (*f).write(cchar(&BIT), 1);
    (*f).write(cchar(&db->BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    S = Dallocar(f, sizeof(SEGMENT));
    db->TABELAS_HEADER_SEG = Dallocar(f, sizeof(TABLE_HEADER));
    if(!SET_BLOCK(f, cchar(&db->TABELAS_HEADER_SEG), S)) return false;
    return true;
}

bool SET_TABLE_HEADER(fstream *f, TABLE_HEADER *TH, SEGMENT S){
    if(!SET_BLOCK(f, cchar(TH), S)){
        cout("Erro ao inserir Cabeçalho da TabelaTabela");
        return false;
    }
    return true;
}

bool SET_TABLE(fstream *f, TABLE *T, SEGMENT S){
    if(!SET_BLOCK(f, cchar(T), S)){
        cout("Erro ou inserir Tabela");
        return false;
    }
    return true;
}

bool SET_MDADO(fstream *f, M_DADO *M, TABLE_HEADER *TH, SEGMENT S){
    SEGMENT ax;
    ax.BASE = S.BASE;
    ax.LIMIT = sizeof(SEGMENT);
    if(!SET_BLOCK(f, cchar(&M->NOME), ax)){
        cout("Ocorreu um erro ao tente inserir o Meta Dado");
        delete[] M->NOMES;
        return false;
    }
    ax.BASE += ax.LIMIT;
    if(!SET_BLOCK(f, cchar(&M->TIPOS), ax)){
        cout("Ocorreu um erro ao tente inserir o Meta Dado");
        delete[] M->NOMES;
        return false;
    }
    ax.BASE += ax.LIMIT;
    ax.LIMIT = sizeof(SEGMENT)*TH->NUMERO_COLUNAS;
    if(!SET_BLOCK(f, cchar(M->NOMES), ax)){
        cout("Ocorreu um erro ao tente inserir o Meta Dado");
        delete[] M->NOMES;
        return false;
    }
    delete[] M->NOMES;
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

bool COMPARAR(void *p, void *p2, char TIPO){
    if(TIPO == INT) return *cint(p)==*cint(p2);
    else if(TIPO == FLOAT) return *cfloat(p)==*cfloat(p2);
    else if(TIPO == DOUBLE) return *cdouble(p)==*cdouble(p2);
    else if(TIPO == CHAR) return *cchar(p)==*cchar(p2);
    else if(TIPO == STRING) return *cstring(p)==*cstring(p2);
    cout("Tipo informado errado");
    return false;
}

void IMPRIMIR_MR(vector<MEM_REGISTER> *v, string md){
    for(unsigned int i=0; i<(*v).size(); i++){
        for(unsigned int j=0; j<md.size(); j++){
            if(md[j] == INT){
                cout((*v)[i].vars[j].nome<<" : "<<*cint((*v)[i].vars[j].info));
            }
            else if(md[j] == FLOAT) {
                cout((*v)[i].vars[j].nome<<" : "<<*cfloat((*v)[i].vars[j].info));
            }
            else if(md[j] == DOUBLE) {
                cout((*v)[i].vars[j].nome<<" : "<<*cdouble((*v)[i].vars[j].info));
            }
            else if(md[j] == CHAR) {
                cout((*v)[i].vars[j].nome<<" : "<<*cchar((*v)[i].vars[j].info));
            }
            else if(md[j] == STRING) {
                cout((*v)[i].vars[j].nome<<" : "<<*cstring((*v)[i].vars[j].info));
            }
        }
        cout("\n\n\n");
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
    M_DADO M_DADOS[NT];
    if(!ABRIR_ARQUIVO(&DBASE, db->NOME, SAIDA)) return false;
    if(!SET_DB_HEADER(&DBASE, db)) return false;
    if(!FILL_TABLE_HEADER(&DBASE, &TH[0], stoi(CONTR[0][1]))) return false;
    if(!SET_TABLE_HEADER(&DBASE, &TH[0], db->TABELAS_HEADER_SEG)) return false;

    for(int i=1;i<NT-1;i++)
        if(!(FILL_TABLE_HEADER(&DBASE, &TH[i], stoi(CONTR[i][1]))&&SET_TABLE_HEADER(&DBASE, &TH[i], TH[i-1].NEXT))) return false;

    TH[NT-1].NUMERO_COLUNAS = stoi(CONTR[NT-1][1]);
    TH[NT-1].NUMERO_REGISTROS = 0;
    TH[NT-1].TABLE_POSITION = Dallocar(&DBASE, sizeof(TABLE));
    TH[NT-1].NEXT.BASE = TH[NT-1].NEXT.LIMIT = -1;
    SET_TABLE_HEADER(&DBASE, &TH[NT-1], TH[NT-2].NEXT);

    for(int i=0; i<NT; i++)
        if(!(FILL_TABLE(&DBASE, &TABELAS[i], &TH[i])&&SET_TABLE(&DBASE, &TABELAS[i], TH[i].TABLE_POSITION))) return false;

    for(int i=0; i<NT; i++)
        if(!(FILL_MDADO(&DBASE, &M_DADOS[i], &TH[i], &CONTR[i])&&SET_MDADO(&DBASE, &M_DADOS[i], &TH[i], TABELAS[i].META_TABLE_POSITION))) return false;

    DBASE.close();
    return true;
}

bool VALID_SEG(SEGMENT S){
    return(S.BASE!=-1&&S.LIMIT!=-1);
}

bool MDADO_TO_STRING(M_DADO *M, vector<string> *s, fstream *f){
    /*
        retorna os tipos ("s,i,s"); [0]
        e os nomes das variaveis. [1...TAM];
    */
    (*s)[0].resize(M->TIPOS.LIMIT);
    if(!GET_BLOCK(f, cchar((*s)[0].c_str()), M->TIPOS)){
        cout("Ocorreu um erro ao tentar transformar o metadado");
        return false;
    }
    for(unsigned int i=1; i<(*s).size(); i++){
        (*s)[i].resize(M->NOMES[i-1].LIMIT);
        if(!GET_BLOCK(f, cchar((*s)[i].c_str()), M->NOMES[i-1])){
            cout("Ocorreu um erro ao tentar transformar o metadado");
            return false;
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

bool LER_MDADO(fstream *f, M_DADO *M, TABLE_HEADER *TH, SEGMENT POS){
    SEGMENT SEG;
    SEG.BASE = POS.BASE;
    SEG.LIMIT = sizeof(SEGMENT);
    if(!GET_BLOCK(f, cchar(&M->NOME), SEG)){
        cout("Ocorreu um erro");
        return false;
    }
    SEG.BASE+=sizeof(SEGMENT);
    if(!GET_BLOCK(f, cchar(&M->TIPOS), SEG)){
        cout("Ocorreu um erro");
        return false;
    }
    M->NOMES = new SEGMENT[TH->NUMERO_COLUNAS];
    SEG.BASE += sizeof(SEGMENT);
    SEG.LIMIT = sizeof(SEGMENT)*TH->NUMERO_COLUNAS;
    if(!GET_BLOCK(f, cchar(M->NOMES), SEG)){
        cout("Ocorreu um erro");
        delete[] M->NOMES;
        return false;
    }
    return true;
}

bool LER_REGISTRO(fstream *f, REGISTRO *R, TABLE_HEADER *TH, SEGMENT POS){
    R->DADOS = new SEGMENT[TH->NUMERO_COLUNAS];
    POS.LIMIT-=sizeof(SEGMENT);
    if(!GET_BLOCK(f, cchar(R->DADOS), POS)){
        cout("Ocorreu um erro ao ler o registro");
        delete[] R->DADOS;
        return false;
    }
    POS.BASE += POS.LIMIT;
    POS.LIMIT = sizeof(SEGMENT);
    if(!GET_BLOCK(f, cchar(&R->NEXT), POS)){
        cout("Ocorreu um erro ao ler o registro");
        delete[] R->DADOS;
        return false;
    }
    return true;
}

bool RtoMR(fstream *f, vector<string> *s, REGISTRO *R, MEM_REGISTER *MR){
    node n;
    for(unsigned int i=0; i<(*s)[0].size(); i++){
        n.nome = (*s)[i+1];
        n.info = Malocar((*s)[0][i]);
        if((*s)[0][i]=='s'){
            (*cstring(n.info)).resize(R->DADOS[i].LIMIT);
            if(!GET_BLOCK(f, cchar((*cstring(n.info)).c_str()), R->DADOS[i])){
                cout("Ocorreu um erro ao transformar o registro");
                return false;
            }
        }
        else if(!GET_BLOCK(f, cchar(n.info), R->DADOS[i])){
            cout("Ocorreu um erro ao transformar o registro");
            return false;
        }
        MR->vars.push_back(n);
    }
    return true;
}
//=======================================================================================================
bool CRIAR_DATABASE(string NOME, vector<string>CONTR[], int N_TAB){
    DATABASE db;
    db.NOME = NOME;
    db.BASE_HEADER.NUMERO_TABELAS = N_TAB;
    CONTROL(&db,CONTR);
    return true;
}

bool INSERIR_REGISTRO(DATABASE *db, MEM_REGISTER REGISTER){
    /*if(!db->OPEN){
        cout("ERRO! O banco nao estah aberto");
        return false;
    }*/
    fstream *DBASE;
    SEGMENT SH, P;
    TABLE_HEADER TH;
    TABLE TABELA;
    M_DADO MDADO;
    REGISTRO R;
    DBASE = OPEN_DATABASE(db);
    if(DBASE==nullptr) return false;
    if(!LER_TABLE_HEADER(DBASE, &TH, db->TABELAS_HEADER_SEG)){
        DBASE->close();
        delete DBASE;
        return false;
    }
    SH = db->TABELAS_HEADER_SEG;
    for(int i=0; i<db->BASE_HEADER.NUMERO_TABELAS; i++){
        if(!LER_TABELA(DBASE, &TABELA, TH.TABLE_POSITION)){
            DBASE->close();
            delete DBASE;
            return false;
        }
        if(!LER_MDADO(DBASE, &MDADO, &TH, TABELA.META_TABLE_POSITION)){
            DBASE->close();
            delete DBASE;
            return false;
        }
        string s;
        s.resize(MDADO.NOME.LIMIT);
        if(!GET_BLOCK(DBASE, cchar(s.c_str()), MDADO.NOME)){
            DBASE->close();
            delete DBASE;
            delete[] MDADO.NOMES;
            return false;
        }
        if(s==REGISTER.nome) break;
        SH = TH.NEXT;
        if(!LER_TABLE_HEADER(DBASE, &TH, TH.NEXT)){
            cout("ERRO! Tabela não encontrada");
            DBASE->close();
            delete DBASE;
            delete[] MDADO.NOMES;
            return false;
        }
    }
    SEGMENT SEG = Dallocar(DBASE, sizeof(SEGMENT)*(TH.NUMERO_COLUNAS+1));
    if(VALID_SEG(TABELA.REGISTROS)){
        P = TABELA.REGISTROS;
        if(!LER_REGISTRO(DBASE, &R, &TH, TABELA.REGISTROS)){
            DBASE->close();
            delete DBASE;
            return false;
        }
        while(VALID_SEG(R.NEXT)){
            P = R.NEXT;
            if(!LER_REGISTRO(DBASE, &R, &TH, R.NEXT)){
                DBASE->close();
                delete DBASE;
                return false;
            }
        }
        R.NEXT = SEG;
        DBASE->seekg((P.BASE+P.LIMIT)-sizeof(SEGMENT));
        DBASE->write(cchar(&R.NEXT), sizeof(SEGMENT));
        TH.NUMERO_REGISTROS++;
        SET_TABLE_HEADER(DBASE, &TH, SH);
    }
    else{
        TABELA.REGISTROS = SEG;
        SET_TABLE(DBASE, &TABELA, TH.TABLE_POSITION);
        TH.NUMERO_REGISTROS++;
        SET_TABLE_HEADER(DBASE, &TH, SH);
        R.DADOS = nullptr;
    }
    P.BASE = P.LIMIT = -1;
    if(R.DADOS != nullptr) delete[] R.DADOS;
    R.NEXT = P;
    R.DADOS = new SEGMENT[TH.NUMERO_COLUNAS];
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&MDADO, &s, DBASE)){
        DBASE->close();
        delete DBASE;
        delete[] R.DADOS;
        return false;
    }
    for(unsigned int i=0; i<REGISTER.vars.size(); i++){
        R.DADOS[i] = P;
        for(int j=0; j<TH.NUMERO_COLUNAS; j++){
            if(s[j+1]==REGISTER.vars[i].nome){
                R.DADOS[j] = Dallocar(DBASE, TSize(s[0][j]));
                if(!VALID_SEG(R.DADOS[j])) R.DADOS[j] = Dallocar(DBASE, (*cstring(REGISTER.vars[i].info)).size());
                if(s[0][j]==STRING) SET_BLOCK(DBASE, cchar((*cstring(REGISTER.vars[j].info)).c_str()), R.DADOS[j]);
                else SET_BLOCK(DBASE, cchar(REGISTER.vars[j].info), R.DADOS[j]);
            }
        }
    }
    DBASE->seekg(SEG.BASE);
    DBASE->write(cchar(R.DADOS), sizeof(SEGMENT)*TH.NUMERO_COLUNAS);
    DBASE->write(cchar(&R.NEXT), sizeof(SEGMENT));
    DBASE->close();
    delete[] R.DADOS;
    delete DBASE;
    return true;
}

vector<MEM_REGISTER> PEGAR_REGISTRO(DATABASE *db, string CONTROLE[3], void **p){
    /*
        CONTROLE[0] -> Nome da tabela
        Se CONTROLE[1] for "ID" então devo retornar o CONTROLE[2] ésimo registro
        Caso não, será o nome da variavel que tem que ser igual a *p
        Se CONTROLE[1] for "*" Deve retornar todos os registros
    */
    fstream *DBASE;
    REGISTRO R;
    vector<MEM_REGISTER> MRL;
    TABLE_HEADER TH;
    TABLE TABELA;
    M_DADO MDADO;
    DBASE = OPEN_DATABASE(db);
    if(DBASE==nullptr) return MRL;
    if(!LER_TABLE_HEADER(DBASE, &TH, db->TABELAS_HEADER_SEG)) return MRL;
    if(CONTROLE[1]=="ID") if(stoi(CONTROLE[2])>TH.NUMERO_REGISTROS) return MRL;
    if(!LER_TABELA(DBASE, &TABELA, TH.TABLE_POSITION)) return MRL;
    while(true){
        if(!LER_MDADO(DBASE, &MDADO, &TH, TABELA.META_TABLE_POSITION)) return MRL;
        char s[MDADO.NOME.LIMIT+1];
        if(!GET_BLOCK(DBASE, s, MDADO.NOME));
        s[MDADO.NOME.LIMIT]='\0';
        string ax=s;
        if(ax==CONTROLE[0]) break;
        if(TH.NEXT.BASE==-1) return MRL;
        if(!(LER_TABLE_HEADER(DBASE, &TH, TH.NEXT)&&LER_TABELA(DBASE, &TABELA, TH.TABLE_POSITION))) return MRL;

    }
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    if(!MDADO_TO_STRING(&MDADO, &s, DBASE)) return MRL;
    MEM_REGISTER MR;
    if(CONTROLE[1]=="ID"){
        int n=stoi(CONTROLE[2]);
        if(!LER_REGISTRO(DBASE, &R, &TH, TABELA.REGISTROS)) return MRL;
        for(int i=1; i<n;i++) if(!LER_REGISTRO(DBASE, &R, &TH, R.NEXT)) return MRL;
        if(!RtoMR(DBASE, &s, &R, &MR)) return MRL;
        MRL.push_back(MR);
    }else if(CONTROLE[1]=="*"){
        if(!LER_REGISTRO(DBASE, &R, &TH, TABELA.REGISTROS)) return MRL;
        for(int i=0; i<TH.NUMERO_REGISTROS; i++){
            if(!RtoMR(DBASE, &s, &R, &MR)){
                DBASE->close();
                delete DBASE;
                MRL.clear();
                return MRL;
            }
            MRL.push_back(MR);
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, &TH, R.NEXT)){
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
    else if(*p!=nullptr){
        int n;
        for(n=1; n<=TH.NUMERO_COLUNAS; n++) if(s[n]==CONTROLE[1]) break;
        if(n>TH.NUMERO_COLUNAS) return MRL;
        if(!LER_REGISTRO(DBASE, &R, &TH, TABELA.REGISTROS)) return MRL;
        for(int i=0; i<TH.NUMERO_REGISTROS;i++){
            if(!RtoMR(DBASE, &s, &R, &MR)){
                DBASE->close();
                delete DBASE;
                MRL.clear();
                return MRL;
            }
            if(COMPARAR(MR.vars[n-1].info, *p, s[0][n-1])) MRL.push_back(MR);
            if(VALID_SEG(R.NEXT)){
                if(!LER_REGISTRO(DBASE, &R, &TH, R.NEXT)){
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
    *p=Malocar('s');
    *cstring(*p)=s[0];
    DBASE->close();
    delete DBASE;
    delete[] MDADO.NOMES;
    //delete[] R.DADOS;
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
    return DBASE;
}

bool ABRIR_ARQUIVO(fstream *f, string NOME, char TIPO_ABERTURA){
    if(TIPO_ABERTURA == ENTRADA) (*f).open(NOME, fstream::in);
    else if(TIPO_ABERTURA == SAIDA) (*f).open(NOME, fstream::out);
    else if(TIPO_ABERTURA == ENTRADA_SAIDA) (*f).open(NOME, fstream::in|fstream::out);
    return (*f).is_open();
}
