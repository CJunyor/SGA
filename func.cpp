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

int Abrir(tabela *tabela){
    fstream fd;
    int coln;
    string ax;
    cout("Digite o nome da Tabela:");
    cin>>ax;
    fd.open(ax, ifstream::in);
    if(!fd.is_open()){
        cout("Nome Invalido");
        return 0;
    }
    fd.read(cchar(&tabela->idmax),sizeof(int));
    fd.read(cchar(&coln),sizeof(int));
    for(int i=0;i<coln;i++){
        no x;
        char tipo;
        int tam;
        fd.read((char *)&tipo, sizeof(char));
        fd.read((char *)&tam, sizeof(int));
        char nome[tam+1];
        fd.read(nome, tam);
        nome[tam]='\0';
        x.dado.tipo=tipo;
        x.dado.nome=nome;
        tabela->colunas.push_back(x);
    }
    for(int i=0;i<tabela->idmax;i++){
        for(list<no>::iterator i=tabela->colunas.begin();i!=tabela->colunas.end();i++){
            if(i->dado.tipo==INT){
                int d;
                fd.read((char *)&d, sizeof(int));
                i->info.push_back(Mallocar(i->dado.tipo,cvoid(&d)));
            }
            else if(i->dado.tipo==CHAR) {
                char d;
                fd.read((char *)&d, sizeof(char));
                i->info.push_back(Mallocar(i->dado.tipo,cvoid(&d)));
            }
            else if(i->dado.tipo==FLOAT){
                float d;
                fd.read((char *)&d, sizeof(float));
                i->info.push_back(Mallocar(i->dado.tipo,cvoid(&d)));
            }
            else if(i->dado.tipo==DOUBLE){
                double d;
                fd.read((char *)&d, sizeof(double));
                i->info.push_back(Mallocar(i->dado.tipo,cvoid(&d)));
            }
            else if(i->dado.tipo==STRING){
                int tam;
                fd.read((char *)&tam, sizeof(int));
                char s[tam+1];
                string d;
                fd.read(s, tam);
                s[tam]='\0';
                d=s;
                i->info.push_back(Mallocar(i->dado.tipo,cvoid(&d)));
            }
        }
    }
    tabela->nome=ax;
    fd.close();
    return 1;
}

void ImprimeTabela(tabela* tabela){
    int cont=0;
    cout("\n\n          "<<tabela->nome<<"\n\n__________________________________________\n");
    cout<<"|  ID  |";
    for(no x:tabela->colunas){
        cout<<"|\t"<<x.dado.nome<<"\t|";
    }
    cout("");
    if(tabela->idmax>0)cout<<"|  1  |";
    for(list<no>::iterator i=tabela->colunas.begin();cont<tabela->idmax;){
        if((int)i->info.size()>=cont){
            if((*i).dado.tipo==INT) cout<<"|  "<<*cint(i->info[cont])<<"  |";
            else if(i->dado.tipo==CHAR) cout<<"|  "<<*cchar(i->info[cont])<<"  |";
            else if(i->dado.tipo==FLOAT) cout<<"|  "<<*cfloat(i->info[cont])<<"  |";
            else if(i->dado.tipo==DOUBLE) cout<<"|  "<<*cdouble(i->info[cont])<<"  |";
            else if(i->dado.tipo==STRING) cout<<"|  "<<*cstring(i->info[cont])<<"  |";
        }
        if(i==tabela->colunas.end()){
                i=tabela->colunas.begin();
                cont++;
                cout("");
                if(cont!=tabela->idmax)cout<<"|  "<<cont+1<<"  |";
        }else i++;
    }
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

void CriarTabela(tabela *tabela){
    string ax;
    no x;
    cout("Digite o nome da tabela:");
    cin>>ax;
    tabela->nome=ax;
    ax="";
    while(ax!="-1"){
        cout("Digite o tipo da variavel:");
        cin>>ax;
        if(ax=="-1") break;
        x.dado.tipo=ax[0];
        cout("Digite o nome da variavel:");
        cin>>ax;
        x.dado.nome=ax;
        tabela->colunas.push_back(x);
    }
    tabela->idmax=0;
}

void Atribuir(tabela *tabela){
    string s;
    list<no>::iterator i;
    cout("1 - Alterar\n2 - Adicionar");
    cin>>s;
    if(s=="1"){
        while(1){
            cout("Digite o nome do coluna que vc quer alterar:");
            cin>>s;
            for(i=tabela->colunas.begin();i!=tabela->colunas.end();i++) if((*i).dado.nome==s) break;
            if(i==tabela->colunas.end()) cout("Nome de coluna invalido!");
            else break;
        }
        int sz;
        cout("Digite o ID:");
        cin>>sz;
        if(((int)i->info.size()>=sz)){
            Desaloc(i->dado.tipo, i->info[sz-1]);
            cout("Digite o valor");
            i->info[sz-1]=Alloc(i->dado.tipo);
        }
        else {
            cout("ID invalido");
        }
    }
    else if(s=="2"){
        for(list<no>::iterator i=tabela->colunas.begin();i!=tabela->colunas.end();i++){
            cout("Digite o valor para a a coluna "<<i->dado.nome<<":");
            i->info.push_back(Alloc(i->dado.tipo));
        }
        tabela->idmax++;
    }
}

void Salvar(tabela *tabela){
    fstream fd;
    int tam;
    //string mtabela;
    fd.open(tabela->nome, ofstream::out);
    if(!(fd.is_open())) {
        cout("Nao foi possivel salvar");
        exit(1);
    }
    tam=tabela->colunas.size();
    fd.write((char *)&tabela->idmax,sizeof(int));
    fd.write((char *)&tam,sizeof(int));
    for(no x :tabela->colunas){
        char tipo=x.dado.tipo;
        fd.write(cchar(&tipo), sizeof(char));
        tam=x.dado.nome.size();
        fd.write(cchar(&tam), sizeof(int));
        fd.write(cchar(x.dado.nome.c_str()), tam);
    }
    for(int x=0;x<(int)tabela->colunas.size();x++){
        for(list<no>::iterator i=tabela->colunas.begin();i!=tabela->colunas.end();i++){
            char tipo=i->dado.tipo;
            if(tipo==INT) {
                fd.write(cchar(i->info[x]), sizeof(int));
            }
            else if(tipo==CHAR) {
                fd.write((char *)i->info[x], sizeof(char));
            }
            else if(tipo==FLOAT){
                fd.write((char *)i->info[x], sizeof(float));
            }
            else if(tipo==DOUBLE){
                fd.write((char *)i->info[x], sizeof(double));
            }
            else if(tipo==STRING){
                tam=(*cstring(i->info[x])).size();
                fd.write((char *)&tam, sizeof(int));
                fd.write((char *)(*cstring(i->info[x])).c_str(), tam);
            }
        }
    }
    fd.close();
    cout("Salvo com sucesso");
}

int SubMenu(tabela *tabela){
    while(1){
        ImprimeSubMenu();
        char c;
        cin>>c;
        switch(c){
            case '1':
                ImprimeTabela(tabela);
                Atribuir(tabela);
                break;
            case '2':
                ImprimeTabela(tabela);
                break;
            case '3':
                Salvar(tabela);
                break;
            case '4':
                delete tabela;
                return 1;
        }
    }
    return 0;
}

int Iniciar(tabela *t){
    while(1){
        t=new tabela;
        ImprimeMenu();
        char c;
        cin>>c;
        switch(c){
            case '1':
                Abrir(t);
                SubMenu(t);
                break;
            case '2':
                CriarTabela(t);
                SubMenu(t);
                break;
            case '3':
                return 0;
        }
    }
    return 0;
}


//=======================================================================================================


DATABASE *CRIAR_DATABASE(string NOME, vector<string>CONTR[], int N_TAB){
    DATABASE *db=new DATABASE;
    db->NOME=NOME;
    db->BASE_HEADER.NUMERO_TABELAS=N_TAB;
    CONTROL(db,CONTR);
    return db;
}

bool INSERIR_REGISTRO(DATABASE *db, MEM_REGISTER REGISTER){
    /*if(!db->OPEN){
        cout("ERRO! O banco nao estah aberto");
        return false;
    }*/
    fstream DBASE;
    char BIT;
    int p;
    TABLE_HEADER *TABLE_H = new TABLE_HEADER;
    TABLE *TABELA = new TABLE;
    M_DADO *MDADO = new M_DADO;
    REGISTRO *REG = new REGISTRO;
    DBASE.open(db->NOME, fstream::in|fstream::out);
    if(!DBASE.is_open()){
        cout("Nao foi possivel assaltar o banco");
        DBASE.close();
        return false;
    }
    DBASE.read(cchar(&BIT), 1);
    DBASE.read(cchar(&db->BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    DBASE.read(cchar(&db->TABELAS_HEADER_SEG), sizeof(SEGMENT));
    DBASE.seekg(db->TABELAS_HEADER_SEG.BASE);
    DBASE.read(cchar(TABLE_H), db->TABELAS_HEADER_SEG.LIMIT);
    for(int i=0; i<db->BASE_HEADER.NUMERO_TABELAS; i++){
        DBASE.seekg(TABLE_H->TABLE_POSITION.BASE);
        DBASE.read(cchar(TABELA), TABLE_H->TABLE_POSITION.LIMIT);
        DBASE.seekg(TABELA->META_TABLE_POSITION.BASE);
        DBASE.read(cchar(&MDADO->NOME), sizeof(SEGMENT));
        DBASE.read(cchar(&MDADO->TIPOS), sizeof(SEGMENT));
        char ax[MDADO->NOME.LIMIT+1];
        DBASE.seekg(MDADO->NOME.BASE);
        DBASE.read(ax, MDADO->NOME.LIMIT);
        ax[MDADO->NOME.LIMIT]='\0';
        if(ax==REGISTER.nome) break;
        DBASE.seekg(TABLE_H->NEXT.BASE);
        DBASE.read(cchar(TABLE_H), TABLE_H->NEXT.LIMIT);
    }
    if(TABLE_H->NEXT.BASE==-1){
        cout("ERRO! Tabela não encontrada");
        DBASE.close();
        return false;
    }
    MDADO->NOMES = new SEGMENT[TABLE_H->NUMERO_COLUNAS];
    DBASE.seekg(TABELA->META_TABLE_POSITION.BASE+2*sizeof(SEGMENT));
    DBASE.read(cchar(MDADO->NOMES), sizeof(SEGMENT)*TABLE_H->NUMERO_COLUNAS);
    SEGMENT *SEG=Dallocar(&DBASE, sizeof(SEGMENT)*(TABLE_H->NUMERO_COLUNAS+1));
    REG->DADOS=new SEGMENT[TABLE_H->NUMERO_COLUNAS];
    if(TABELA->REGISTROS.BASE!=-1){
        DBASE.seekg(TABELA->REGISTROS.BASE);
        DBASE.read(cchar(REG->DADOS), TABLE_H->NUMERO_COLUNAS);
        DBASE.read(cchar(&REG->NEXT), sizeof(SEGMENT));
        while(REG->NEXT.BASE!=-1){
            p=DBASE.tellg();
            DBASE.read(cchar(REG->DADOS), TABLE_H->NUMERO_COLUNAS);
            DBASE.read(cchar(&REG->NEXT), sizeof(SEGMENT));
        }
        REG->NEXT = *SEG;
        DBASE.seekg(p+sizeof(SEGMENT)*TABLE_H->NUMERO_COLUNAS);
        DBASE.write(cchar(&REG->NEXT), sizeof(SEGMENT));
    }else{
        TABELA->REGISTROS=*SEG;
        DBASE.seekg(TABLE_H->TABLE_POSITION.BASE);
        DBASE.write(cchar(TABELA), sizeof(TABLE));
    }
    REGISTRO ax;
    ax.NEXT.BASE=-1;
    ax.NEXT.LIMIT=-1;
    ax.DADOS = new SEGMENT[TABLE_H->NUMERO_COLUNAS];
    string *s=MDADO_TO_STRING(MDADO, TABLE_H->NUMERO_COLUNAS, &DBASE);
    for(int i=0; i<(int) REGISTER.vars.size(); i++){
        ax.DADOS[i].BASE=-1;
        ax.DADOS[i].LIMIT=-1;
        for(int j=0; j<TABLE_H->NUMERO_COLUNAS; j++){
            if(s[j+1]==REGISTER.vars[i].nome){
                SEGMENT SEG3 = *Dallocar(&DBASE, TSize(s[0][j]));
                if(SEG3.BASE==-1) SEG3 = *Dallocar(&DBASE, (*cstring(REGISTER.vars[i].info)).size());
                cout(SEG3.BASE);
                cout(SEG3.LIMIT);
                ax.DADOS[j]=SEG3;
                DBASE.seekg(SEG3.BASE);
                if(s[0][j]==STRING) DBASE.write((*cstring(REGISTER.vars[j].info)).c_str(), SEG3.LIMIT);
                else DBASE.write(cchar(REGISTER.vars[j].info), SEG3.LIMIT);
            }
        }
    }
    DBASE.seekg(SEG->BASE);
    DBASE.write(cchar(ax.DADOS), sizeof(SEGMENT)*TABLE_H->NUMERO_COLUNAS);
    DBASE.write(cchar(&ax.NEXT), sizeof(SEGMENT));
    DBASE.close();
    return true;
}

SEGMENT *Dallocar(fstream *f, int TAM){
    SEGMENT *S = new SEGMENT;
    if(TAM>0){
        char lixo[TAM];
        (*f).seekg(0, fstream::end);
        S->BASE=(*f).tellg();
        S->LIMIT=TAM;
        (*f).write(cchar(lixo), TAM);
    }else{
        S->BASE=-1;
        S->LIMIT=-1;
    }
    return S;
}

int TSize(char TIPO){
    if(TIPO==INT) return sizeof(int);
    else if(TIPO==CHAR) return sizeof(char);
    else if(TIPO==FLOAT) return sizeof(float);
    else if(TIPO==DOUBLE) return sizeof(double);
    else return -1;
}


fstream *OPEN_DATABASE(DATABASE *db, string nome){
    char BIT;
    fstream  *DBASE=new fstream;
    (*DBASE).open(db->NOME, fstream::in);
    if(!(*DBASE).is_open()){
        cout("Nao fo pssivel abrir o banco");
        return nullptr;
    }
    (*DBASE).read(cchar(&BIT), 1);
    if(BIT!=BIT_PROTETOR){
        cout("ERRO! Este arquivo nao e valido");
        return nullptr;
    }
    (*DBASE).read(cchar(&db->BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    (*DBASE).read(cchar(&db->TABELAS_HEADER_SEG), sizeof(SEGMENT));
    return DBASE;
}

string *MDADO_TO_STRING(M_DADO *M, int TAM, fstream *f){
    string *s = new string[TAM+1];
    char ax[M->TIPOS.LIMIT+1];
    (*f).seekg(M->TIPOS.BASE);
    (*f).read(ax, M->TIPOS.LIMIT);
    ax[M->TIPOS.LIMIT]='\0';
    s[0]=ax;
    for(int i=0; i<TAM; i++){
        char ax[M->NOMES[i].LIMIT+1];
        (*f).seekg(M->NOMES[i].BASE);
        (*f).read(ax, M->NOMES[i].LIMIT);
        ax[M->NOMES[i].LIMIT]='\0';
        s[i+1] = ax;

    }
    return s;
}

bool CONTROL(DATABASE *db, vector<string>CONTR[]){
    fstream DBASE;
    char BIT=BIT_PROTETOR;
    int TAB_N=db->BASE_HEADER.NUMERO_TABELAS;
    TABLE_HEADER *TABLE_H=new TABLE_HEADER[TAB_N];
    TABLE *TABELAS=new TABLE[TAB_N];
    M_DADO *M_DADOS=new M_DADO[TAB_N];
    int P=5;
    DBASE.open(db->NOME, fstream::out);
    if(!DBASE.is_open()) return false;
    DBASE.write(cchar(&BIT), 1);
    DBASE.write(cchar(&TAB_N), sizeof(int));
    P+=sizeof(SEGMENT);
    cout(P);
    db->TABELAS_HEADER_SEG.BASE=P;
    db->TABELAS_HEADER_SEG.LIMIT=sizeof(TABLE_HEADER);
    DBASE.write(cchar(&db->TABELAS_HEADER_SEG), sizeof(SEGMENT));
    cout(DBASE.tellp());
    int P2=P+sizeof(TABLE_HEADER);
    P+=TAB_N*sizeof(TABLE_HEADER);
    cout(P);
    for(int i=0;i<TAB_N;i++){
        TABLE_H[i].NUMERO_COLUNAS=stoi(CONTR[i][1]);
        TABLE_H[i].NUMERO_REGISTROS=0;
        TABLE_H[i].TABLE_POSITION.BASE=P;
        TABLE_H[i].TABLE_POSITION.LIMIT=sizeof(TABLE);
        TABLE_H[i].NEXT.BASE=P2;
        TABLE_H[i].NEXT.LIMIT=sizeof(TABLE_HEADER);
        P+=sizeof(TABLE);
        P2+=sizeof(TABLE_HEADER);
    }
    TABLE_H[TAB_N-1].NEXT.BASE=-1;
    TABLE_H[TAB_N-1].NEXT.LIMIT=-1;
    DBASE.write(cchar(TABLE_H), TAB_N*sizeof(TABLE_HEADER));
    cout(DBASE.tellp());
    //P+=TAB_N*sizeof(TABLE);
    cout(P);
    for(int i=0;i<TAB_N;i++){
        TABELAS[i].META_TABLE_POSITION.BASE=P;
        TABELAS[i].META_TABLE_POSITION.LIMIT=sizeof(SEGMENT)*(TABLE_H[i].NUMERO_COLUNAS+2);
        TABELAS[i].REGISTROS.BASE=-1;
        TABELAS[i].REGISTROS.LIMIT=-1;
        P+=sizeof(SEGMENT)*(TABLE_H[i].NUMERO_COLUNAS+2);
    }
    DBASE.write(cchar(TABELAS), TAB_N*sizeof(TABLE));
    cout(DBASE.tellp());
    for(int i=0;i<TAB_N;i++){
        M_DADOS[i].NOME.BASE=P;
        M_DADOS[i].NOME.LIMIT=CONTR[i][0].size();
        P+=CONTR[i][0].size();
        M_DADOS[i].TIPOS.BASE=P;
        M_DADOS[i].TIPOS.LIMIT=CONTR[i][2].size();
        P+=CONTR[i][2].size();
        M_DADOS[i].NOMES=new SEGMENT[TABLE_H[i].NUMERO_COLUNAS];
        for(int j=0;j<TABLE_H[i].NUMERO_COLUNAS;j++){
            M_DADOS[i].NOMES[j].BASE=P;
            M_DADOS[i].NOMES[j].LIMIT=CONTR[i][j+3].size();
            P+=CONTR[i][j+3].size();
        }
        DBASE.write(cchar(&M_DADOS[i].NOME), sizeof(SEGMENT));
        DBASE.write(cchar(&M_DADOS[i].TIPOS), sizeof(SEGMENT));
        DBASE.write(cchar(M_DADOS[i].NOMES), sizeof(SEGMENT)*TABLE_H[i].NUMERO_COLUNAS);
    }
    for(int i=0;i<TAB_N;i++){
        DBASE.write(CONTR[i][0].c_str(), (int)CONTR[i][0].size());
        DBASE.write(CONTR[i][2].c_str(), (int)CONTR[i][2].size());
        for(int j=0;j<TABLE_H[i].NUMERO_COLUNAS;j++){
            DBASE.write(CONTR[i][j+3].c_str(), (int)CONTR[i][j+3].size());
        }
    }
    DBASE.close();
    return true;
}
