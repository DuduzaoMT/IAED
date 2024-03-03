#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

/* Mostra uma paragem. */
void mostraParagem(Paragem *p) {

    printf("%s: %16.12f %16.12f %d\n",p->nome,p->latitude, 
            p->longitude, p->numCarreiras);
}

/* Mostra todas as paragens. */
void listaParagens(list_paragens **l) {

    Paragem *a;
    for (a = (*l)->head; a != NULL; a = a->next){
        if (strlen(a->nome)>10)
            mostraParagem(a);
    }
}

/* Verifica se existe uma paragem com um determinado nome.
   Se existir devolve a estrutura correspondente. */
Paragem* encontraParagem(list_paragens **l,char *nomeParagem) {

    Paragem *a;

    for (a = (*l)->head; a != NULL; a = a->next)
        if (strcmp(nomeParagem, a->nome) == 0)
            return a;
    return NULL;
}

/* dado o indice da paragem devolve a estrutura correspondente */
Paragem* encontraParagemInd(list_paragens *lp,int indiceParagem){

    Paragem * p;
    if (lp->head != NULL && lp->last != NULL){
        for(p = lp->head ; p != NULL; p = p->next){
            if (p->idparagem == indiceParagem)
                return p;
        }
    }
    return NULL;
}

/* Cria uma nova paragem. */
void criaParagem(list_paragens **l, char *nomeParagem, double latitude, double longitude) {

    Paragem *new = (Paragem*)malloc(sizeof(Paragem));
    static int idparagem = 0;

    verificaMalloc(new);
    /* inicializa tudo a 'zero' e associa o identificador unico */
    new->idparagem = idparagem++;
    new->latitude = latitude;
    new->longitude = longitude;
    new->numCarreiras = 0;
    new->head = NULL;
    new->tail = NULL;
    new->nome = (char*)malloc(sizeof(char)*(strlen(nomeParagem)+1));
    verificaMalloc(new->nome);
    strcpy(new->nome,nomeParagem);

    /* associa à lista de paragens(fim) */
    new->next = NULL;
    if ((*l)->last != NULL)
        (*l)->last->next = new;
    (*l)->last = new;
    if ((*l)->head == NULL)
        (*l)->head = new;
}

/* Função para tratar o comando 'p'. */
void paragens(list_paragens **l) {

    char *s;
    int fimLinha = leEspacos();
    Paragem *p;
    
    if (!fimLinha) {
        listaParagens(l);
        return;
    }
    leNome(&s);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if ((p = encontraParagem(l,s)) == NULL)
            printf("%s: no such stop.\n", s);
        else {
            printf("%16.12f %16.12f\n", p->latitude, p->longitude);
        }
    }
    else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (encontraParagem(l,s) == NULL)
            criaParagem(l,s, latitude, longitude);
        else
            printf("%s: stop already exists.\n", s);
        leAteFinalLinha();
    }
    free(s);
}


/* Funções comando 'e' */

/* associa à nova lista (final) */
void adicionaLigacaoNovaLista(Ligacao **Lig,Ligacao **head,Ligacao **tail){

    (*Lig)->next = NULL;
    (*Lig)->prev = (*tail); 

    if ((*tail) != NULL)
        (*tail)->next = (*Lig);
    (*tail) = (*Lig);
    if ((*head) == NULL)
        (*head) = (*Lig);
}

/* encontra 'match' para a origem (paragem)*/
int matchOrigem(Ligacao **LigInicial,Paragem *p){

    Ligacao *aux;
/* guarda destinos para se comparar depois(no caso de ser o caso particular de 2 paragens) */
    int Atual = p->idparagem;
    /* procura a primeira paragem de origem diferente de p */
    while ((*LigInicial)!= NULL && (*LigInicial)->idOrigem == p->idparagem){
        aux = (*LigInicial);
        (*LigInicial) = (*LigInicial)->next;
        Atual = aux->idDestino;
        free(aux);
    }
    return Atual;
}

/* encontra 'match' para destino (paragem)*/
void matchDestino(Ligacao *LigInicial,Ligacao **LigFinal,Paragem *p,double*custo,double *duracao){

    Ligacao *aux;

    /* começar onde a outra parou */
    (*LigFinal) = LigInicial;
    /* encontra a primeira ligação cuja paragem de destino é diferente da que queremos remover */
    while ((*LigFinal) != NULL && (*LigFinal)->idDestino == p->idparagem){
        *custo += (*LigFinal)->custo;
        *duracao += (*LigFinal)->duracao;
        aux = (*LigFinal);
        (*LigFinal) = (*LigFinal)->next;
        free(aux);
    }
}

/* Paragem 'p' a remover da lista de ligações de 'c' e criar uma nova lista */

/* idealmente a ideia será pegar na primeira paragem de ORIGEM diferente daquela que queremos remover,
encontrar a paragem de DESTINO diferente daquela que queremos revomer e criar uma nova ligação 
(repetir este ciclo o numero de vezes que for preciso) */

/* retorna nao_encontrado caso nao satisfaça o caso particular de ter apenas duas paragens e a carreira ficar a null*/
int criaNovaListaLigacoes(Carreira *c,Paragem *p,Ligacao **nova_head, Ligacao **nova_tail){

    Ligacao *LigInicial = c->head,*LigFinal = c->tail,*NovaLigacao;
    int ParagemO,ParagemD,Atual;
    double custoLigacao,duracaoLigacao;
    double *ptrCusto = &custoLigacao,*ptrDuracao=&duracaoLigacao;

    while( LigInicial != NULL && LigFinal != NULL){

        /* encontra o 'match' para a origem */
        Atual = matchOrigem(&LigInicial,p);

        /* no caso em que a Ligação fica a NULL e a carreira tem duas paragens, temos de remover 
            a carreira dessa outra paragem tambem */
        if (LigInicial==NULL){
            if (Atual != p->idparagem && (*nova_head) == NULL)
                return Atual;
            return NAO_ENCONTRADO;
        }
        ParagemO = LigInicial->idOrigem;
        /* reset custos e ligação e encontra 'match' para o destino */
        custoLigacao = 0;duracaoLigacao = 0;
        matchDestino(LigInicial,&LigFinal,p,ptrCusto,ptrDuracao);

        /* no caso em que a Ligação fica a NULL e a carreira tem duas paragens, temos de remover 
            a carreira dessa outra paragem tambem */
        if (LigFinal==NULL){
            if ((*nova_head) == NULL && ParagemO != p->idparagem)
                return ParagemO;
            return NAO_ENCONTRADO;
        }
        /* soma de custos e durações da ultima */
        custoLigacao += LigFinal->custo;
        duracaoLigacao += LigFinal->duracao;

        /* criação e associação da ligação */
        ParagemD = LigFinal->idDestino;
        NovaLigacao = criaLigacao(ParagemO,ParagemD,custoLigacao,duracaoLigacao);
        adicionaLigacaoNovaLista(&NovaLigacao,nova_head,nova_tail);
        LigInicial = LigFinal->next;
        free(LigFinal);
    }
    return NAO_ENCONTRADO;
}

/* recalcula o custo e duração total da carreira */
void atualizaCarreira(Carreira **c){

    Ligacao *l;
    /* resetar os contadores */
    (*c)->custoTotal = 0;
    (*c)->duracaoTotal = 0;
    (*c)->numLigacoes = 0;

    for (l = (*c)->head; l != NULL; l = l->next){
        (*c)->custoTotal += l->custo;
        (*c)->duracaoTotal += l->duracao;
        (*c)->numLigacoes +=1;
    }
}

/* reestrutura a lista de paragens pela remoção de p */
void ReestruturaListaParagens(Paragem **p,list_paragens **lp){

    Paragem *temp;

    /* mais de um elemento */
    if ((*lp)->head->next != NULL){
        if ((*lp)->head->idparagem != (*p)->idparagem){
            /* procura o elemento cujo o seu sucessor seja aquele que queremos remover */
            for (temp = (*lp)->head; temp->next->idparagem != (*p)->idparagem ; temp = temp->next);

            if (temp->next->next == NULL)
                (*lp)->last = temp;
            temp->next = temp->next->next;
        }
        else {
            (*lp)->head = (*lp)->head->next;
        } 
    }
    else{
        (*lp)->head = NULL;
        (*lp)->last = NULL;
    }
}

/* associa a nova head e a nova tail */
void associaHT(Carreira **c,Ligacao **head,Ligacao **tail){
    (*c)->head = (*head);
    (*c)->tail = (*tail);
}

/* Função que da free da lista de copias de carreiras da paragem */
void FreeListCarParagem(Paragem **p){
    Carreira *c = (*p)->head,*aux;

    while(c!= NULL){
        aux = c;
        c = c->next;
        free(aux->nome);
        free(aux);
    }
}

/* Função comando 'e' */
void eliminaParagem(list_paragens ** lp,list_carreira **lc){

    Paragem *p,*aux;
    Carreira *c,*carreira;
    Ligacao *nova_head,*nova_tail;
    char *nomeParagem;
    int indice;

    leEspacos();
    leNome(&nomeParagem);
    leAteFinalLinha();

    if((p = encontraParagem(lp,nomeParagem)) == NULL){
        printf("%s: no such stop.\n",nomeParagem);
        return;
    }
    for(c = p->head; c != NULL; c = c->next){
        /* reset às novas head e tail*/
        nova_head = NULL; nova_tail = NULL;
        carreira = encontraCarreiraInd((*lc),c->idcarreira);

        indice = criaNovaListaLigacoes(carreira,p,&nova_head,&nova_tail);
        /* se a carreira ficar sem ligações tenho de ir à outra paragem e tira-la de la 
        (caso de duas paragens apenas) */
        if (indice != NAO_ENCONTRADO){
            aux = encontraParagemInd((*lp),indice);
            removeCarreiraParagem(&c,&aux);
        }
        /* associa a nova lista de ligações */
        associaHT(&carreira,&nova_head,&nova_tail);
        atualizaCarreira(&carreira);
    }
    /* free de tudo e tirar a paragem da lista */
    FreeListCarParagem(&p);
    ReestruturaListaParagens(&p,lp);
    free(nomeParagem);
    free(p->nome);
    free(p);
}
