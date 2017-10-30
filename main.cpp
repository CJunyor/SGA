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
    */
    fstream DBASE;
    DBASE.open("TESTE", ofstream::in);
    char bit;
    DATABASE db;
    TABLE_HEADER *TABLE_H=new TABLE_HEADER[2];
    TABLE *TABELAS=new TABLE[2];
    M_DADO *M_DADOS=new M_DADO[3];
    db.NOME="TESTE";
    DBASE.read(cchar(&bit),1);
    DBASE.read(cchar(&db.BASE_HEADER.NUMERO_TABELAS), sizeof(int));
    db.TABELAS_HEADER_SEG=new SEGMENT[db.BASE_HEADER.NUMERO_TABELAS];
    DBASE.read(cchar(db.TABELAS_HEADER_SEG), sizeof(SEGMENT)*db.BASE_HEADER.NUMERO_TABELAS);
    DBASE.seekg(db.TABELAS_HEADER_SEG[0].BASE,DBASE.beg);
    DBASE.read(cchar(TABLE_H), db.TABELAS_HEADER_SEG[0].LIMIT);
    DBASE.seekg(TABLE_H[0].TABLE_POSITION.BASE, DBASE.beg);
    DBASE.read(cchar(TABELAS), TABLE_H[0].TABLE_POSITION.LIMIT);
    DBASE.seekg(TABELAS[0].META_TABLE_POSITION.BASE, DBASE.beg);
    DBASE.read(cchar(&M_DADOS[0].NOME), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[0].TIPOS), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[0].NOMES), sizeof(SEGMENT)*TABLE_H[0].NUMERO_COLUNAS);
    DBASE.read(cchar(&M_DADOS[1].NOME), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[1].TIPOS), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[1].NOMES), sizeof(SEGMENT)*TABLE_H[0].NUMERO_COLUNAS);
    DBASE.read(cchar(&M_DADOS[2].NOME), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[2].TIPOS), sizeof(SEGMENT));
    DBASE.read(cchar(&M_DADOS[2].NOMES), sizeof(SEGMENT)*TABLE_H[0].NUMERO_COLUNAS);
    DBASE.seekg(M_DADOS[0].NOME.BASE,DBASE.beg);
    char s[M_DADOS[0].NOME.LIMIT+1];
    DBASE.read(s, M_DADOS[0].NOME.LIMIT);
    cout(M_DADOS[0].NOME.BASE);
    s[M_DADOS[0].NOME.LIMIT]='\0';
    cout(s);
    DBASE.close();
    return 0;
}
