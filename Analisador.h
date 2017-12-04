#ifndef ANALISADOR_H_INCLUDED
#define ANALISADOR_H_INCLUDED
/* Fun��o onde printa a query e diz cada tipo de palavra que foi digitado */
void printal();

/* Verifica se � um indentificador */
int isInd(char ch);

/* Verifica se � um operador*/
int isOp(char ch);

/* Verificar se � um Delimitador */
int isDel(char ch);

/* Verifica se � uma chave */
int isKey(char * Query);

/* Analisa a quary separando as strings e indetificando cada uma para pode fazer a manipula��o */
void analise(char *Query);

void createDB();
void comando();

const char * ficarBig(char []);
#endif // ANALISADOR_H_INCLUDED
