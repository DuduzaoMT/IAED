/* iaed-23 - ist1106929 - project2 */

/*
 * File:  main.c
 * Author:  Eduardo Silva
 * Description: O meu ficheiro .c com a resolução do problema proposto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h" /* import das structs do projeto */

/* apaga o sistema completo */
void apagaSistema(list_paragens **lp,list_carreira **lc){

    Carreira *auxC;
    Paragem *auxP;

    /* free de todas as carreiras e suas ligações */
    while((*lc)->head != NULL){
        auxC = (*lc)->head;
        (*lc)->head = (*lc)->head->next;
        freeLigCarreira(&auxC);
        free(auxC->nome);
        free(auxC);
    }
    (*lc)->head = NULL;
    (*lc)->last = NULL;

    /* free de todas as paragens */
    while((*lp)->head != NULL){
        auxP = (*lp)->head;
        FreeListCarParagem(&(*lp)->head);
        (*lp)->head = (*lp)->head->next;
        free(auxP->nome);
        free(auxP);
    }
    (*lp)->head = NULL;
    (*lp)->last = NULL;
}

/* Função MAIN */
int main() {
    int c;
    list_carreira *_carreiras = list_build_c();
    list_paragens *_paragem = list_build_p();

    do {
        c = getchar();
        switch(c) {
            case 'c':
                carreiras(&_paragem,&_carreiras);
                break;
            case 'p':
                paragens(&_paragem);
                break;
            case 'l':
                ligacoes(&_carreiras,&_paragem);
                break;
            case 'i':
                intersecoes(&_paragem);
                break;
            case 'r':
                removeCarreira(&_carreiras,&_paragem);
                break;
            case 'e':
                eliminaParagem(&_paragem,&_carreiras);
                break;
            case 'q':
                apagaSistema(&_paragem,&_carreiras);
                free(_carreiras);
                free(_paragem);
                break;
            case 's':
                NovoPrint(&_carreiras,&_paragem);
                break;
            case 'a':
                apagaSistema(&_paragem,&_carreiras);
                break;
            default:
	        /* Ignorar linhas em branco */
	        if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');
    return 0;
}