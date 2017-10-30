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

DATABASE *CRIAR_DATABASE(string NOME, vector<string>CONTR[], int N_TAB){
    DATABASE *db=new DATABASE;
    db->NOME=NOME;
    db->BASE_HEADER.NUMERO_TABELAS=N_TAB;
    db->TABELAS_HEADER_SEG=new SEGMENT[N_TAB];
    CONTROL(db,CONTR);
    return db;
}

bool CONTROL(DATABASE *db, vector<string>CONTR[]){
    fstream DBASE;
    char BIT=BIT_PROTETOR;
    int TAB_N=db->BASE_HEADER.NUMERO_TABELAS;
    TABLE_HEADER *TABLE_H=new TABLE_HEADER[TAB_N];
    TABLE *TABELAS=new TABLE[TAB_N];
    M_DADO *M_DADOS=new M_DADO[TAB_N];
    int P=5;
    DBASE.open(db->NOME, ofstream::out);
    if(!DBASE.is_open()) return false;
    DBASE.write(cchar(&BIT), 1);
    DBASE.write(cchar(&TAB_N), sizeof(int));
    P+=TAB_N*sizeof(SEGMENT);
    cout(P);
    for(int i=0;i<TAB_N;i++){
        db->TABELAS_HEADER_SEG[i].BASE=P;
        db->TABELAS_HEADER_SEG[i].LIMIT=sizeof(TABLE_HEADER);
        P+=sizeof(TABLE_HEADER);
    }
    DBASE.write(cchar(db->TABELAS_HEADER_SEG), TAB_N*sizeof(SEGMENT));
    cout(DBASE.tellp());
    //P+=TAB_N*sizeof(TABLE_HEADER);
    cout(P);
    for(int i=0;i<TAB_N;i++){
        TABLE_H[i].NUMERO_COLUNAS=stoi(CONTR[i][1]);
        TABLE_H[i].NUMERO_REGISTROS=0;
        TABLE_H[i].TABLE_POSITION.BASE=P;
        TABLE_H[i].TABLE_POSITION.LIMIT=sizeof(TABLE);
        P+=sizeof(TABLE);
    }
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
