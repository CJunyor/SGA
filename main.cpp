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
#include "Analisador.h"







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
    CRIAR_DATABASE("teste.mydb",c,2);
    //*/

    //*
    fstream f;
    f.open("Comando.txt", fstream::in);
    if(!f.is_open()){
        cout("Nao foi possivel abrir o arquivo");
        return 0;
    }
    string Comando;
    while(!f.eof()){
        while(getline(f, Comando) && Comando[0]=='-');
        cout("Comando: "<<Comando<<"\n");
        analise(cchar(Comando.c_str()));
        comando();
    }
    //*/

    /*
    node ax;
    MEM_REGISTER R;
    DATABASE db;
    R.nome="Alunos";
    ax.info=new string;
    ax.nome="Nome";
    *cstring(ax.info)="Pedro";
    R.vars.push_back(ax);
    ax.info=new string;
    ax.nome="Matricula";
    *cstring(ax.info)="321654987";
    R.vars.push_back(ax);
    ax.info = new int;
    ax.nome = "Idade";
    *cint(ax.info) = 87;
    R.vars.push_back(ax);

    db.NOME="teste.mydb";
    INSERIR_REGISTRO(&db, R);


    //*/


    /*
    DATABASE db;
    string s[3];
    void *p=nullptr;
    db.NOME="teste.mydb";
    s[0]="Alunos";
    s[1]="*";
    s[2]="";
    p = Malocar(STRING);
    *cstring(p)="Diego";
    vector<MEM_REGISTER> v=PEGAR_REGISTRO(&db, s, &p);
    IMPRIMIR_MR(&v, *cstring(p));
    //delete cstring(p);
    return 0;
    //*/


    /*
    DATABASE db;
    db.NOME = "teste.mydb";
    fstream *f=OPEN_DATABASE(&db);
    REGISTRO R;
    MEM_REGISTER mr;
    vector<MEM_REGISTER> v;
    TABLE_HEADER TH;
    TABLE T;
    META_DADO M;
    SEGMENT SEG, SEG2;
    int  p;
    SEG = PEGAR_TABLE_INDEX(&db, &TH, &T, &M, "Alunos");
    BTree ax = criarNo();
    carregaBTree(cchar(db.NOME.c_str()), &ax, SEG);
    ax = buscaChave(ax, 2, &p);
    SEG = ax->chave[p].seek;
    cout(SEG.BASE);

    LER_REGISTRO(f, &R,SEG);
    vector<string> s;
    s.resize(TH.NUMERO_COLUNAS+1);
    cout(SEG.BASE);
    MDADO_TO_STRING(&M, &s, f);
    cout(SEG.BASE);
    cout(s[0]);
    cout(SEG.BASE);
    RtoMR(f, &s, &R, &mr);
    v.push_back(mr);
    IMPRIMIR_MR(&v, s[0]);
    //*/
    return 0;

}
