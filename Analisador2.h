#ifndef ANALISADOR_H_INCLUDED
#define ANALISADOR_H_INCLUDED
#define N_KEYS 14
/* Fun��o onde printa a query e diz cada tipo de palavra que foi digitada */
void printal(char [][50], char [][8], char [][50], char [20], char [20], int [5]);

/* Verifica se � um indentificador */
int isInd(char ch);

/* Verifica se � um operador*/
int isOp(char ch);

/* Verificar se � um Delimitador */
int isDel(char ch);

/* Verifica se � uma chave */
int isKey(char []);

/* Analisa a quary separando as strings e indetificando cada uma para pode fazer a manipula��o */
void analise(char []);

void createDB();
void comando(char [], char [][50], char [][8], char [][50], char [20], char [20], int [5]);

void ficarBig(char [], char []);
#endif // ANALISADOR_H_INCLUDED
