#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

/* função de verificação do malloc */
void verificaMalloc(void*l){
    if(l==NULL){
        printf("No memory.\n");
        exit(MEMORY_FAIL);
    }
}

/* Funções para parsing do input */

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */
void leNome(char **palavra) {
    int i = 0, c;
    char s[MAX];

    s[0] = getchar();
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';

    (*palavra) = (char*)malloc(sizeof(char)*(strlen(s)+1));
    verificaMalloc((*palavra));
    strcpy((*palavra),s);
}

/* Le todo o texto até ao final de linha. */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n');
}

