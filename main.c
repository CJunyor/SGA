#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Analisador.h"


int main()
{
  char Query[50];

  printf("\nInsira uma Query: ");
  scanf("%[^\n]c",Query);

  analise(Query);
  comando();

  return 0;
}


