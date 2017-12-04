#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Analisador.h"
#include "func.h"



char    operadores[2] = {
			'*','='
		};

char	delimitador[4] = {
			';','(',')',','
		};

char    *wordKey[] = {
			"create","table","database","update",
			"select","set","where","insert","into","values"
        };

int     t1=0, t2=0, k, t3=0, t4=0, t5=0, t6=0,TamChaves=0,
		TamOperadores=0, TamK=0, TamLiterais=0, TamConst=0, l=0, j;

char    QueryDel[20], tIden[20][20], tOp[20][20], Chave[20][20],
		iden[20][20], oper[20][20], delem[20], Literail[20][20], lit[20];


void comando()
{
	if(strcmp(iden[0], "create" ) == 0 )
	{
		if(strcmp(iden[1], "database" ) == 0 ){
			createDB();
		}else if(strcmp(iden[1], "table" ) == 0 ){
			printf("\nTabela criada com sucesso!\n");
		}
	}else if(strcmp(ficarBig(iden[0]), "INSERT") == 0 ){
        if(!strcmp(ficarBig(iden[1]), "ON")){
            DATABASE db;
            db.NOME = iden[2];
            db.NOME.append(".mydb");
            if(!strcmp(ficarBig(iden[3]), "INTO")){
                MEM_REGISTER MR;
                MR.nome = iden[4];
                if(!strcmp(ficarBig(iden[5]), "VALUES")){
                    string s = PEGAR_REG_TIPO(&db, MR.nome);
                    if(s.empty()) return;
                    for(int k=0; k<TamLiterais; k++){
                        node ax;
                        string s2 = Literail[k];
                        if(!Atribuir(&ax, s2, s[k])) return;
                        //ax.nome = Literail[k];
                        MR.vars.push_back(ax);
                    }
                    if(INSERIR_REGISTRO(&db, MR)){
                        cout("Inserido com sucesso");
                    }

                }
            }
        }
	}else if(strcmp(iden[0], "update" ) == 0 ){

	}else if(!strcmp(ficarBig(iden[0]), "SELECT" )){
        if(!strcmp(ficarBig(iden[1]), "ON" )){
            DATABASE db;
            string s[3];
            void *p = nullptr;
            db.NOME = iden[2];
            db.NOME.append(".mydb");
            if(!strcmp(ficarBig(iden[3]),"FROM")){
                s[0] = iden[4];
                if(!strcmp(oper[0],"*")) {
                    s[1] = "*";
                    vector<MEM_REGISTER> v = PEGAR_REGISTRO(&db, s, &p);
                    IMPRIMIR_MR(&v, *cstring(p));
                    delete cstring(p);
                }

            }
        }
	}else{
		printf("\n[ %s ] Comando Invalido!\n",iden[0]);
	}
}

void createDB(){

	/* MyQuery: create database func;
	Para cria??o de um banco de dado; */

	char nomeBanco[30];

	strcpy(nomeBanco,iden[2]);
	strcat(nomeBanco,".txt"); //puts(nomeBanco);

	FILE *a;
	a = fopen(nomeBanco,"w");
    if(a == NULL){
        printf("Erro na abertura!\n\n");
		exit(1);
    }
    printf("\nBanco de dados [ %s ] foi criado com sucesso!\n",nomeBanco);

}

void analise(char *Query)
{
	int i=0;
    t1=0;
    t3=0;
    t5=0;
    TamLiterais =0;
    l=0;
    iden[t1][t2]='\0';
	while(Query[i]!='\0')
	{
		/* Armazena todas as palavras que não são delimitadores, operadores e literais. Sabe que é uma chave chamando a tabela wordKey e verificando
		   para pode fazer a manipulação e validação dos comandos */
		if(isInd(Query[i]))
		{
			while(isInd(Query[i]))
			{
				iden[t1][t2++] = Query[i++];
			}
			iden[t1][t2]='\0';
			t1++;
			t2=0;
		}else
			/* Verifica se é uma string e armazena em literais para fazer a manipulação dos dados, pega os valores entre aspas */
			if(Query[i]=='"')
			{
				i+=1;
				lit[l++] = Query[i];
				for(j=i+1; Query[j]!='"';j++)
				{
					lit[l++] = Query[j];
				}
				//lit[l++] = Query[j];
				lit[l]='\0';
				strcpy(Literail[TamLiterais++], lit);
				lit[0]='\0';
				l=0;
				i=j+1;
			}else
				/* Para Operadores verifica se é um operadore reservado da tabela */
				if(isOp(Query[i]))
				{
					while(isOp(Query[i]))
					{
						oper[t3][t4++] = Query[i++];
					}
					oper[t3][t4]='\0';
					t3++; t4=0;
				}else
					/* Verificando se é uma delimitador */
					if(isDel(Query[i]))
					{
						while(isDel(Query[i]))
						{
							delem[t5++]=Query[i++];
						}
					}else{
						i++;
					}
    }
    //printal();
}

int isInd(char ch)
{
    if(isalpha(ch) || ch=='_' || isdigit(ch) || ch=='.') return 1;
    return 0;
}

int isOp(char ch)
{
  	int f=0, i;
	for(i=0; i<8 && !f; i++)
	{
	   if(ch == operadores[i])
	    f=1;
	}
 return f;
}

int isDel(char ch)
{
  	int f=0, i;
	for(i=0; i<4 && !f ; i++)
	{
		if(ch == delimitador[i]) f=1;
	}
 	return f;
}

int isKey(char* Query)
{
  int i, f=0;
  for(i=0; i<10; i++)
  {
    if(!strcmp(wordKey[i],Query))  f=1;
  }
  return f;
}


void printal()
{
	int i=0;

	/* Verificando na tabela de chaves se ela existe */
	for(i=0; i<TamChaves; i++)
	{
		if(isKey(tIden[i]))
		    strcpy(Chave[TamK++],tIden[i]);
		else
			strcpy(iden[t1++],tIden[i]);
	}

	printf("\nPalavras Chaves: \n");
	for(i=0;i<TamK;puts(Chave[i]),i++);

}
const char * ficarBig(char s[]){
    for(unsigned int i=0; i<strlen(s); i++){
        s[i] = toupper(s[i]);
    }
    return s;
}
