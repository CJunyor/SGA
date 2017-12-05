#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Analisador.h"
#include "func.h"

void comando(char Query[], char Identificadores[][50], char KEYS[][8], char Literais[][50],
             char Operadores[20], char Delimitadores[20], int Tam[5])
{
	if(Tam[1]>=1 && !strcmp(KEYS[0], "CREATE" ))
	{
		if(Tam[1]>=2 && !strcmp(KEYS[1], "DATABASE" )){
            if(Tam[0]>=1){
                strcat(Identificadores[0], ".mydb");
                if(CRIAR_DATABASE_VAZIO(Identificadores[0])) cout("Banco criado");
            }
		}
		else if(Tam[1]>=2 && !strcmp(KEYS[1], "TABLE" )){
            if(Tam[0]>=1){
                string nome = Identificadores[0];
                if(Tam[1]>=3 && !strcmp(KEYS[2], "ON")){
                    if(Tam[0]>=2 && Tam[2] == 0){
                        DATABASE db;
                        db.NOME = Identificadores[1];
                        db.NOME.append(".mydb");
                        if(INSERIR_TABELA_VAZIA(&db, nome)) cout("Tabela inserida");
                    }
                }
            }
		}else if(Tam[1]>=2 && !strcmp(KEYS[1], "COLUMN" )){
            if(Tam[2]>=1){
                string coluna = Literais[0];
                if(Tam[0]>=1){
                    char tipo = tolower(Identificadores[0][0]);
                    if(Tam[1]>=3 && !strcmp(KEYS[2], "ON" )){
                        if(Tam[1]>=4 && !strcmp(KEYS[3], "TABLE" )){
                            if(Tam[0]>=2){
                                string tabela = Identificadores[1];
                                if(Tam[1]>=5 && !strcmp(KEYS[4], "IN" )){
                                    if(Tam[0]>=3){
                                        DATABASE db;
                                        db.NOME = Identificadores[2];
                                        db.NOME.append(".mydb");
                                        if(INSERIR_COLUNA(&db, tabela, coluna, tipo)) cout("Coluna criada");
                                    }
                                }
                            }
                        }
                    }
                }
            }
		}
	}else if(Tam[1]>=1 && !strcmp(KEYS[0], "INSERT")){
        if(Tam[1]>=2 && !strcmp(KEYS[1], "ON")){
            if(Tam[0]>=1){
                DATABASE db;
                db.NOME = Identificadores[0];
                db.NOME.append(".mydb");
                if(Tam[1]>=3 && Tam[0]>=2 && !strcmp(KEYS[2], "INTO")){
                    MEM_REGISTER MR;
                    MR.nome = Identificadores[1];
                    if(Tam[1]>=4 && !strcmp(KEYS[3], "VALUES")){
                        string s = PEGAR_REG_TIPO(&db, MR.nome);
                        if(s.empty()) return;
                        for(int i=0; i<Tam[2]; i++){
                            node ax;
                            string s2 = Literais[i];
                            if(!Atribuir(&ax, s2, s[i])) return;
                            MR.vars.push_back(ax);
                        }
                        if(INSERIR_REGISTRO(&db, MR)) cout("Inserido com sucesso");
                    }
                }
            }
        }
	}else if(Tam[1]>=1 && !strcmp(KEYS[0], "UPDATE")){

	}else if(Tam[1]>=1 && !strcmp(KEYS[0], "SELECT")){
        if(Tam[1]>=3 && Tam[0]>=1 && !strcmp(KEYS[1], "ON" )){
            DATABASE db;
            string s[3];
            void *p = nullptr;
            db.NOME = Identificadores[0];
            db.NOME.append(".mydb");
            if(Tam[0]>=2 &&!strcmp(KEYS[2],"FROM")){
                s[0] = Identificadores[1];
                if(Operadores[0] == '*') {
                    s[1] = "*";
                    vector<MEM_REGISTER> v = PEGAR_REGISTRO(&db, s, &p);
                    IMPRIMIR_MR(&v, *cstring(p));
                    delete cstring(p);
                }
            }
        }
	}else{
		printf("\n[ %s ] Comando Invalido!\n", Query);
	}
}

void analise(char Q[])
{
    char Identificadores[50][50];
    char KEYS[20][8];
    char Literais[50][50];
    char Operadores[20];
    char Delimitadores[20];
    char Query[100];
    int nIdentificadores = 0, nKeys = 0, nLiterais = 0, nOperadores = 0, nDelimitadores = 0;
    strcpy(Query, Q);
	for(int i=0; Query[i]!='\0'; )
	{
		/* Armazena todas as palavras que não são delimitadores, operadores e literais. Sabe que é uma chave chamando a tabela wordKey e verificando
		   para pode fazer a manipulação e validação dos comandos */
		if(isInd(Query[i]))
		{
		    char ax[50], ax2[50];
		    memset(ax, '\0', 50);
		    memset(ax2, '\0', 50);
			for(int j=0; isInd(Query[i]); j++) ax[j] = Query[i++];
			ficarBig(ax, ax2);
			if(isKey(ax2)) strcpy(KEYS[nKeys++], ax2);
			else strcpy(Identificadores[nIdentificadores++], ax);
		}
		else{
			/* Verifica se é uma string e armazena em literais para fazer a manipulação dos dados, pega os valores entre aspas */
			if(Query[i]=='\"')
			{
				char ax[50];
				memset(ax, '\0', 50);
				i++;
				for(int j=0; Query[i]!='\"'; j++) ax[j] = Query[i++];
				i++;
				strcpy(Literais[nLiterais++], ax);
			}
			else{
				/* Para Operadores verifica se é um operadore reservado da tabela */
				if(isOp(Query[i]))
				{
					while(isOp(Query[i])) Operadores[nOperadores++] = Query[i++];
				}
				else{
					/* Verificando se é uma delimitador */
					if(isDel(Query[i]))
					{
						while(isDel(Query[i])) Delimitadores[nDelimitadores++] = Query[i++];
					}
					else i++;
				}
			}
		}
    }
    int Tam[] = {nIdentificadores, nKeys, nLiterais, nOperadores, nDelimitadores};
    printal(Identificadores, KEYS, Literais, Operadores, Delimitadores, Tam);
    comando(Query, Identificadores, KEYS, Literais, Operadores, Delimitadores, Tam);
}

int isInd(char ch)
{
    if(isalpha(ch) || ch=='_' || isdigit(ch) || ch=='.') return 1;
    return 0;
}

int isOp(char ch)
{
    char OPERADORES[2] = {'*', '='};
	for(int i=0; i<2; i++)
	{
	   if(ch == OPERADORES[i]) return 1;
	}
    return 0;
}

int isDel(char ch)
{
    char DELIMITADOR[4] = {';', '(', ')', ','};
	for(int i=0; i<4; i++)
	{
		if(ch == DELIMITADOR[i]) return 1;
	}
 	return 0;
}

int isKey(char Word[])
{
    char WORDKEYS[][9] = {
                        "CREATE", "TABLE", "DATABASE", "UPDATE", "SELECT",
                        "SET", "WHERE", "INSERT", "INTO", "VALUES", "FROM",
                        "ON", "COLUMN", "IN"
                        };
    int i;
    for(i=0; i<N_KEYS; i++)
    {
        if(!strcmp(WORDKEYS[i], Word)) return 1;
    }
    return 0;
}

void printal(char Identificadores[][50], char KEYS[][8], char Literais[][50],
             char Operadores[20], char Delimitadores[20], int Tam[5])
{
	if(Tam[0]){
        printf("\nIdentificadores:\n");
        for(int i=0; i<Tam[0]; i++) printf("\n%s\n", Identificadores[i]);
	}
	if(Tam[1]){
        printf("\nChaves:\n");
        for(int i=0; i<Tam[1]; i++) printf("\n%s\n", KEYS[i]);
	}
	if(Tam[2]){
        printf("\nLiterais:\n");
        for(int i=0; i<Tam[2]; i++) printf("\n%s\n", Literais[i]);
	}
	if(Tam[3]){
        printf("\nOperadores:\n");
        for(int i=0; i<Tam[3]; i++) printf("\n%c\n", Operadores[i]);
	}
	if(Tam[4]){
        printf("\nDelimitadores:\n");
        for(int i=0; i<Tam[4]; i++) printf("\n%c\n", Delimitadores[i]);
	}
}
void ficarBig(char s[], char s2[]){
    for(unsigned int i=0; i<strlen(s); i++){
        s2[i] = toupper(s[i]);
    }
}
