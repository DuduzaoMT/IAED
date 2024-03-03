#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

/* Adiciona uma nova ligação.*/
Ligacao* criaLigacao(int idParagemOrigem, int idParagemDestino,
                 double custo, double duracao) {

    Ligacao *new = (Ligacao*)malloc(sizeof(Ligacao));
    verificaMalloc(new);

    new->idOrigem = idParagemOrigem;
    new->idDestino = idParagemDestino;
    new->custo = custo;
    new->duracao = duracao;

    return new;
}

/*Acrescenta uma nova ligação no fim de uma carreira.*/ 
void acrescentaLigacaoFim(Carreira **idCarreira, Ligacao **Lig) {

    (*Lig)->next = NULL;
    (*Lig)->prev = (*idCarreira)->tail; 

    if ((*idCarreira)->tail != NULL)
        (*idCarreira)->tail->next = (*Lig);
    (*idCarreira)->tail = (*Lig);
    if ((*idCarreira)->head == NULL)
        (*idCarreira)->head = (*Lig);
    (*idCarreira)->custoTotal += (*Lig)->custo;
    (*idCarreira)->duracaoTotal += (*Lig)->duracao;
    (*idCarreira)->numLigacoes++;
}

 /*Acrescenta uma nova ligação no início de uma carreira. */
void acrescentaLigacaoInicio(Carreira **idCarreira, Ligacao **Lig) {

    (*Lig)->next = (*idCarreira)->head;
    (*Lig)->prev = NULL;
    (*idCarreira)->head->prev = (*Lig); 
    (*idCarreira)->head = (*Lig);
    (*idCarreira)->custoTotal += (*Lig)->custo;
    (*idCarreira)->duracaoTotal += (*Lig)->duracao;
    (*idCarreira)->numLigacoes++;
}

/* Adiciona uma nova carreira à lista de carreiras de uma paragem. */
void adicionaCarreiraParagem(Paragem **idParagem, Carreira **Car) {

    Carreira *copy = (Carreira*)malloc(sizeof(Carreira));
    verificaMalloc(copy);
    
    /* associo 'copias' de carreiras à lista (apenas nome e id)*/
    copy->idcarreira = (*Car)->idcarreira;
    copy->next = NULL;

    copy->nome = (char*) malloc(sizeof(char)*(strlen((*Car)->nome)+1)); /* alterei o malloc*/
    verificaMalloc(copy->nome);
    strcpy(copy->nome,(*Car)->nome);

    /* associo ao fim da lista de carreiras que está dentro da paragem */
    if ((*idParagem)->head == NULL && (*idParagem)->tail == NULL){
        (*idParagem)->head = copy;
        (*idParagem)->tail = copy;
    }
    else{
        (*idParagem)->tail->next = copy;
        (*idParagem)-> tail = copy;
    }
    (*idParagem)->numCarreiras++;
}

/* Adiciona primeira ligacao da carreira. */
void adicionaPrimeiraLigacao(Carreira **idCarreira, Paragem *idParagemOrigem, Paragem* idParagemDestino,
                     double custo, double duracao) {

    Ligacao *lig;

    adicionaCarreiraParagem(&idParagemOrigem, idCarreira);
    if (idParagemOrigem->idparagem != idParagemDestino->idparagem)
        adicionaCarreiraParagem(&idParagemDestino, idCarreira);
    lig = criaLigacao(idParagemOrigem->idparagem,idParagemDestino->idparagem, custo, duracao);
    acrescentaLigacaoFim(idCarreira,&lig);  
}

/* Adiciona uma nova ligação a uma carreira. */
void adicionaLigacao(Carreira** idCarreira, 
                        Paragem *idParagemOrigem, Paragem *idParagemDestino, double custo, double duracao) {

    int numLigacoesCarreira = (*idCarreira)->numLigacoes;
    Ligacao *l,*d;
    if (numLigacoesCarreira == 0) 
        adicionaPrimeiraLigacao(idCarreira, idParagemOrigem, idParagemDestino,
				custo, duracao);
    else {
        int ParagemInicio = (*idCarreira)->head->idOrigem;
        int ParagemFim = (*idCarreira)->tail->idDestino;

        /* acrescenta ligação ao fim */
        if (idParagemOrigem->idparagem == ParagemFim) {
            if (encontraParagemCarreira(idCarreira, idParagemDestino->idparagem) == NAO_ENCONTRADO)
                adicionaCarreiraParagem(&idParagemDestino, idCarreira);
            l = criaLigacao(idParagemOrigem->idparagem, idParagemDestino->idparagem, custo, duracao);
            acrescentaLigacaoFim(idCarreira,&l);
        }
        /* acrescenta ligação ao inicio */
        else if (idParagemDestino->idparagem == ParagemInicio) {
            if (encontraParagemCarreira(idCarreira, idParagemOrigem->idparagem) == NAO_ENCONTRADO)
                adicionaCarreiraParagem(&idParagemOrigem,idCarreira);
            d = criaLigacao(idParagemOrigem->idparagem, idParagemDestino->idparagem, custo, duracao);
            acrescentaLigacaoInicio(idCarreira,&d);
        }
        else 
            printf("link cannot be associated with bus line.\n");
    }
}

/* Le nome de carreira e paragens de uma ligacao.*/
void leNomesComando(char **nomeCarreira, char **nomeOrigem, char **nomeDestino) {
    leEspacos();
    leNome(nomeCarreira);
    leEspacos();
    leNome(nomeOrigem);
    leEspacos();
    leNome(nomeDestino);
}

/*Função para tratar o comando 'l'. */
void ligacoes(list_carreira **lc,list_paragens **lp){

    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    double custo, duracao;
    Carreira *indexCarreira;
    Paragem *indexOrigem, *indexDestino;

    leNomesComando(&nomeCarreira, &nomeOrigem, &nomeDestino);
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha();

    indexCarreira = encontraCarreira(lc,nomeCarreira);
    if (indexCarreira == NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        indexOrigem = encontraParagem(lp,nomeOrigem);
        if (indexOrigem == NULL) 
	        printf("%s: no such stop.\n", nomeOrigem);
        else {
            indexDestino = encontraParagem(lp,nomeDestino);
            if (indexDestino == NULL)
                printf("%s: no such stop.\n", nomeDestino);
            else if (custo < 0.0 || duracao < 0.0)
                    printf("negative cost or duration.\n");
            else 
                adicionaLigacao(&indexCarreira, indexOrigem, indexDestino, custo, duracao);
        }
    }
    free(nomeCarreira);
    free(nomeOrigem);
    free(nomeDestino);
}
