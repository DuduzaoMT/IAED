/* iaed-23 - ist1106929 - project2 */

/*
 * File:  project2.h
 * Author:  Eduardo Silva
 * Description: O meu ficheiro .h que contem as 
 *          structs e prototipos para auxilio no projecto.
*/

#ifndef PROJECT2
#define PROJECT2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definicao Constantes */

#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO -1      /* caso nao seja encontrado o caso que procuro */
#define MEMORY_FAIL -2         /* caso não haja espaço na memoria */

#define MAX 65535 /* maximo de caracteres do input */

/* Definicao de Tipos de Dados */

typedef struct ligacao {
    int idOrigem, idDestino;
    double custo, duracao;
    struct ligacao *next,*prev;
} Ligacao;

typedef struct carreira {
    char *nome;
    double custoTotal;
    double duracaoTotal;
    Ligacao *head,*tail;
    int numLigacoes;
    struct carreira *next;
    int idcarreira; /* identificador uniquivoco de cada carreira */
} Carreira;

typedef struct paragem {
    char *nome; 
    double latitude, longitude;
    Carreira *head,*tail;
    int numCarreiras;
    struct paragem *next;
    int idparagem; /* identificador uniquivoco de cada paragem */
} Paragem;

typedef struct  {
    Paragem *head, *last;
} list_paragens;

typedef struct  {
    Carreira *head, *last;
} list_carreira;

/* função de verificação do malloc */
void verificaMalloc(void*l);

/* Funções para parsing do input */
int leEspacos();
void leNome(char **palavra);
void leAteFinalLinha();

/*Funções de procura por indice*/
Paragem* encontraParagemInd(list_paragens *lp,int indiceParagem);
Carreira* encontraCarreiraInd(list_carreira *lc,int indiceCarreira);

/* Funções para tratamento das estuturas do tipo Carreira */
void mostraCarreira(list_paragens **lp,Carreira **c);
void printLigCarreiras(list_paragens **lp,Carreira *c);
void printLigInverso(list_paragens **lp,Carreira *c);
void mostraLigacoesCarreira(list_paragens **lp,Carreira *index, int inverso);
void listaCarreiras(list_paragens **lp,list_carreira **lc);
Carreira* encontraCarreira(list_carreira **lc,char *nomeCarreira); 
int encontraParagemCarreira(Carreira **idCarreira, int idParagem);
int encontraCarreiraParagem(Carreira **head,int idCarreira);
void criaCarreira(list_carreira **l,char *nomeCarreira);
int verificaInversoOk(char *s);
void carreiras(list_paragens **lp,list_carreira **l);
void NovoPrint(list_carreira **lc,list_paragens **lp);

/* Funções para tratamento dos objetos do tipo Paragem */
void mostraParagem(Paragem *p);
void listaParagens(list_paragens **l);
Paragem* encontraParagem(list_paragens **l,char *nomeParagem);
void criaParagem(list_paragens **l, char *nomeParagem, double latitude, 
            double longitude);
void paragens(list_paragens **l);

/* Funções de tratamento de ligações */
Ligacao* criaLigacao(int idParagemOrigem, int idParagemDestino,
                 double custo, double duracao);
void acrescentaLigacaoFim(Carreira **idCarreira, Ligacao **Lig);
void acrescentaLigacaoInicio(Carreira **idCarreira, Ligacao **Lig);
void adicionaCarreiraParagem(Paragem **idParagem, Carreira **Car);
void adicionaPrimeiraLigacao(Carreira **idCarreira, Paragem *idParagemOrigem, 
                Paragem* idParagemDestino,double custo, double duracao);
void adicionaLigacao(Carreira** idCarreira, Paragem *idParagemOrigem, 
                Paragem *idParagemDestino, double custo, double duracao);
void leNomesComando(char **nomeCarreira, char **nomeOrigem, char **nomeDestino);
void ligacoes(list_carreira **lc,list_paragens **lp);

/* Funções para tratamento do comando 'i'*/
void ordenaCarreiras(Carreira **head, int numCarreiras);
void intersecoes(list_paragens **lp);

/* Funções para tratamento do comando 'r'*/
void freeParagem(Carreira *remover,Paragem **paragem);
void removeCarreiraParagem(Carreira **carreira,Paragem **paragem);
void ReestruturaListaCarreiras(list_carreira **lc,Carreira**c);
void comandosRemove(char **nomeCarreira);
void freeLigCarreira(Carreira**c);
void removeCarreira(list_carreira **lc,list_paragens **lp);

/* Funções para tratamento do comando 'e' */
void adicionaLigacaoNovaLista(Ligacao **Lig,Ligacao **head,Ligacao **tail);
int matchOrigem(Ligacao **LigInicial,Paragem *p);
void matchDestino(Ligacao *LigInicial,Ligacao **LigFinal,Paragem *p,
            double*custo,double *duracao);
int criaNovaListaLigacoes(Carreira *c,Paragem *p,Ligacao **nova_head, 
                    Ligacao **nova_tail);
void atualizaCarreira(Carreira **c);
void ReestruturaListaParagens(Paragem **p,list_paragens **lp);
void associaHT(Carreira **c,Ligacao **head,Ligacao **tail);
void FreeListCarParagem(Paragem **p);
void eliminaParagem(list_paragens ** lp,list_carreira **lc);

/* Comando 'a' */
void apagaSistema(list_paragens **lp,list_carreira **lc);

/* criação de listas de estruturas com apontadores para cabeça e cauda */
list_carreira* list_build_c();
list_paragens* list_build_p();

#endif
