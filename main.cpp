#include <iostream>
#include "func.h"

using namespace std;

int main()
{
    /*
    vector<string>c[2];
    c[0].push_back("Alunos");
    c[0].push_back("3");
    c[0].push_back("ssi");
    c[0].push_back("Nome");
    c[0].push_back("Matricula");
    c[0].push_back("Idade");
    c[1].push_back("Carro");
    c[1].push_back("3");
    c[1].push_back("ssf");
    c[1].push_back("Nome");
    c[1].push_back("Marca");
    c[1].push_back("Preco");
    CRIAR_DATABASE("TESTE",c,2);
    //*/
    //parte concluida


    /*
    node ax;
    MEM_REGISTER R;
    DATABASE db;
    R.nome="Alunos";
    ax.info=new string;
    ax.nome="Nome";
    *cstring(ax.info)="Candido";
    R.vars.push_back(ax);
    ax.info=new string;
    ax.nome="Matricula";
    *cstring(ax.info)="123456789";
    R.vars.push_back(ax);
    ax.info = new int;
    ax.nome = "Idade";
    *cint(ax.info) = 19;
    R.vars.push_back(ax);

    db.NOME="TESTE";
    INSERIR_REGISTRO(&db, R);


    //*/



    /*
    fstream DBASE;
    REGISTRO R;
    DBASE.open("TESTE", fstream::in);
    char bit;
    DATABASE db;
    TABLE_HEADER TABLE_H[2];//=new TABLE_HEADER[2];
    TABLE TABELAS[2];//=new TABLE[2];
    M_DADO M_DADOS[3];//=new M_DADO[3];
    db.NOME="TESTE";
    DBASE.read(cchar(&bit),1);
    DBASE.read(cchar(&db.BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    DBASE.read(cchar(&db.TABELAS_HEADER_SEG), sizeof(SEGMENT));
    DBASE.seekg(db.TABELAS_HEADER_SEG.BASE);
    DBASE.read(cchar(TABLE_H), db.TABELAS_HEADER_SEG.LIMIT);
    DBASE.seekg(TABLE_H[0].TABLE_POSITION.BASE);
    DBASE.read(cchar(TABELAS), TABLE_H[0].TABLE_POSITION.LIMIT);
    DBASE.seekg(TABELAS[0].META_TABLE_POSITION.BASE);
    DBASE.read(cchar(&M_DADOS[0].NOME), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[0].TIPOS), sizeof(SEGMENT));
    M_DADOS[0].NOMES=new SEGMENT[TABLE_H[0].NUMERO_COLUNAS];
    DBASE.read(cchar(M_DADOS[0].NOMES), sizeof(SEGMENT)*TABLE_H[0].NUMERO_COLUNAS);
    char s[100];
    string s1[TABLE_H[0].NUMERO_COLUNAS+2];
    DBASE.seekg(M_DADOS[0].NOME.BASE);
    DBASE.read(s, M_DADOS[0].NOME.LIMIT);
    s[M_DADOS[0].NOME.LIMIT]='\0';
    s1[0]=s;
    DBASE.seekg(M_DADOS[0].TIPOS.BASE);
    DBASE.read(s, M_DADOS[0].TIPOS.LIMIT);
    s[M_DADOS[0].TIPOS.LIMIT]='\0';
    s1[1]=s;
    for(int i=0; i<TABLE_H[0].NUMERO_COLUNAS; i++){
        DBASE.seekg(M_DADOS[0].NOMES[i].BASE);
        DBASE.read(s, M_DADOS[0].NOMES[i].LIMIT);
        s[M_DADOS[0].NOMES[i].LIMIT]='\0';
        s1[i+2]=s;
    }
    DBASE.seekg(TABELAS[0].REGISTROS.BASE);
    R.DADOS=new SEGMENT[TABLE_H[0].NUMERO_COLUNAS];
    DBASE.read(cchar(R.DADOS), TABLE_H[0].NUMERO_COLUNAS*sizeof(SEGMENT));
    DBASE.read(cchar(&R.NEXT), sizeof(SEGMENT));
    DBASE.seekg(R.DADOS[0].BASE);
    DBASE.read(s, R.DADOS[0].LIMIT);
    s[R.DADOS[0].LIMIT]='\0';
    cout(s);
    DBASE.seekg(R.DADOS[1].BASE);
    DBASE.read(s, R.DADOS[1].LIMIT);
    s[R.DADOS[1].LIMIT]='\0';
    cout(s);
    int ax;
    DBASE.seekg(R.DADOS[2].BASE);
    DBASE.read(cchar(&ax), R.DADOS[2].LIMIT);
    cout(ax);
    DBASE.close();
    //*/
    return 0;
}
