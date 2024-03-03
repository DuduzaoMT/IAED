#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

/* Funções comando 'c' */

void mostraCarreira(list_paragens **lp,Carreira **c) {

    int numLigacoes = (*c)->numLigacoes;
    int indiceO,indiceD;
    Paragem *pO,*pD;

    printf("%s ",(*c)->nome);
    if (numLigacoes > 0) {
        indiceO = (*c)->head->idOrigem;
        pO = encontraParagemInd((*lp),indiceO);

        indiceD = (*c)->tail->idDestino;
        pD = encontraParagemInd((*lp),indiceD);

        printf("%s %s ", pO->nome,pD->nome);
        printf("%d ", numLigacoes+1);
        printf("%.2f ", (*c)->custoTotal);
        printf("%.2f\n",(*c)->duracaoTotal);
    }
    else 
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/* Mostra as ligações da carreira */
void printLigCarreiras(list_paragens **lp,Carreira *c){

    Ligacao *l;
    Paragem *p,*d;
    int idOrigem,idDestino;

    for (l = c->head; l != NULL; l = l->next) {
        idOrigem = l->idOrigem;
        p = encontraParagemInd((*lp),idOrigem);
        printf("%s, ", p->nome);
    }
    idDestino = c->tail->idDestino;
    d = encontraParagemInd((*lp),idDestino);
    printf("%s\n", d->nome);
}

/* Mostra as ligações por ordem inversa */
void printLigInverso(list_paragens **lp,Carreira *c){

    Ligacao *l;
    Paragem *p,*d;
    int idOrigem,idDestino;

    for (l = c->tail; l != NULL; l = l->prev) {
        idDestino = l->idDestino;
        p = encontraParagemInd((*lp),idDestino);
        printf("%s, ", p->nome);
    }
    idOrigem = c->head->idOrigem;
    d = encontraParagemInd((*lp),idOrigem);
    printf("%s\n", d->nome);
}

/* Função responsavel por verificar qual tipo de print será para efetuar */
void mostraLigacoesCarreira(list_paragens **lp,Carreira *index, int inverso) {

    int numLigacoes = index->numLigacoes;

    if (numLigacoes == 0) 
        return;
    if (inverso == FALSO) {
        printLigCarreiras(lp,index);
    }
    else {
        printLigInverso(lp,index);
    }
}

/* Mostra todas as carreiras. */
void listaCarreiras(list_paragens **lp,list_carreira **lc) {

    Carreira *c;

    for (c = (*lc)-> head; c!=NULL; c = c->next)
        mostraCarreira(lp,&c);
}

/* Procura uma carreira por nome, devolve a estrutura correspondente */
Carreira* encontraCarreira(list_carreira **lc,char *nomeCarreira) {

    Carreira *c;

    for (c = (*lc)->head; c != NULL; c = c->next)
        if (strcmp(c->nome, nomeCarreira) == 0)
            return c;
    return NULL;
}

/* dado o indice da Carreira devolve a estrutura correspondente */
Carreira* encontraCarreiraInd(list_carreira *lc,int indiceCarreira){

    Carreira * c;
    if (lc->head != NULL && lc->last != NULL){
        for(c = lc->head ; c != NULL; c = c->next){
            if (c->idcarreira == indiceCarreira)
                return c;
        }
    }
    return NULL;
}

/* Procura se uma paragem existe numa carreira, se sim devolve o mesmo indice */ 
int encontraParagemCarreira(Carreira **idCarreira, int idParagem) {

    Ligacao *l;
    int id;

    for (l =(*idCarreira)->head; l != NULL; l = l->next) {
        id = l->idOrigem;
        if (idParagem == id)
            return id;
    }
    id = (*idCarreira)->tail->idDestino;
    if (id == idParagem)
        return id;

    return NAO_ENCONTRADO;
}

/* como as paragens têm 'copias' do id, temos de percorrer a lista de carreiras dentro
das paragens e ver se ainda se encontra la esta carreira ou se ja foi removida */
int encontraCarreiraParagem(Carreira **head,int idCarreira){

    Carreira *c;

    for(c = (*head) ; c != NULL; c = c->next){
        if (idCarreira == c->idcarreira)
            return idCarreira;
    }
    return NAO_ENCONTRADO;
    
}

/* Cria uma carreira nova. */
void criaCarreira(list_carreira **l,char *nomeCarreira) {

    Carreira *new = (Carreira*)malloc(sizeof(Carreira));
    static int idcarreira = 0;

    verificaMalloc(new);
    /* inicializa tudo a 'zeros' e associa um identificador unico */
    new->idcarreira = idcarreira++;
    new->custoTotal = 0.0;
    new->duracaoTotal = 0.0;
    new->numLigacoes = 0;
    new->head = NULL;
    new->tail = NULL;

    new->nome = (char*)malloc(sizeof(char*)*(strlen(nomeCarreira)+1));
    verificaMalloc(new->nome);
    strcpy(new->nome, nomeCarreira);

    /* processo de associação ao final da lista de carreiras */
    new->next = NULL;
    if ((*l)->last != NULL)
        (*l)->last->next = new;
    (*l)->last = new;
    if ((*l)->head == NULL)
        (*l)->head = new;
}

/* Verifica se a string é um prefixo de tamanho pelo menos 3 da
   palavra inverso. */
int verificaInversoOk(char *s) {

    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

int verificaNomeCarreira(char *s){
    int c,comp = strlen(s);
    for(c=0;c!=comp;c++){
        if (s[c]<'A' || s[c]>'Z')
            return 0;
    }
    return 1;
}

void NovoPrint(list_carreira **lc,list_paragens **lp){
    char *s;
    Carreira *c;
    Paragem *p;
    int primeira=VERDADE;

    leEspacos();
    leNome(&s);
    leAteFinalLinha();

    if ((p = encontraParagem(lp,s))==NULL){
        printf("%s: no such stop.\n",s);
    }
    else{
        if ((*lc)->head != NULL){
            for (c=(*lc)->head;c!= NULL; c= c->next){
                if (c->numLigacoes >0){
                    if (c->head->idOrigem == p->idparagem){
                        if (primeira == VERDADE){
                            printf("%s",c->nome);
                            primeira = FALSO;
                        }
                        else{
                            printf(" %s",c->nome);
                        }
                    }
                }
            }
        }
        printf("\n");
    }
    free(s);
}
/* Função para tratar do comando 'c'. */
void carreiras(list_paragens **lp,list_carreira **l) {

    char *s;
    int fimLinha = leEspacos();
    Carreira *index;
    
    if (!fimLinha) {
        listaCarreiras(lp,l);
        return;
    }

    leNome(&s);
    index = encontraCarreira(l,s);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if (index == NULL){
            if (verificaNomeCarreira(s))
                criaCarreira(l,s);
            else
                printf("invalid line name.\n");
        }
        else
            mostraLigacoesCarreira(lp,index, FALSO);
    }
    else {
        free(s);
        leNome(&s);
        if (verificaInversoOk(s))
            mostraLigacoesCarreira(lp,index, VERDADE);
        else
            printf("incorrect sort option.\n");
        leAteFinalLinha();
    }
    free(s);
}

/* comando 'i' */

/* Aplica Bubble Sort a um vector de identificadores de carreiras.*/
void ordenaCarreiras(Carreira **head, int numCarreiras) {

    int i, houveTroca = VERDADE;
    Carreira *c, *prev, *aux;

    for (i = 0; houveTroca && i < numCarreiras-1; i++) {
        houveTroca = FALSO;
        prev = NULL;
        for (c = (*head); c != NULL && c->next != NULL; c = aux) {
            aux = c->next;
            if (strcmp(c->nome, aux->nome) > 0) {
                houveTroca = VERDADE;
                if (prev == NULL) {
                    (*head) = aux;
                } 
                else {
                    prev->next = aux;
                }
                c->next = aux->next;
                aux->next = c;
                prev = aux;
            }
            else {
                prev = c;
            }
        }
    }
}

/* Função para tratar o comando 'i'. */
void intersecoes(list_paragens **lp) {

    int numCarreiras;
    Paragem *p;
    Carreira *c;

    leAteFinalLinha();
    for (p = (*lp)->head; p != NULL; p = p->next) {
        numCarreiras = p->numCarreiras;
        if (numCarreiras > 1) {
            printf("%s %d:", p->nome, numCarreiras);
            ordenaCarreiras(&(p->head),p->numCarreiras);
            for (c = p->head; c!= NULL;c = c->next){
                printf(" %s", c->nome);
            }
            printf("\n");
        }
    }
}

/* Funções para o comando 'r' */

/* free da paragem a remover */
void freeParagem(Carreira *remover,Paragem **paragem){

    if (remover != NULL){
        free(remover->nome);
        free(remover);
        (*paragem)->numCarreiras--;
    }
}

/* função que remove e reestrutura a lista de carreiras dentro da paragem */
void removeCarreiraParagem(Carreira **carreira,Paragem **paragem){

    Carreira *c,*remover=NULL;

    /* remove apenas se ela estiver la dentro */
    if (encontraCarreiraParagem(&(*paragem)->head,(*carreira)->idcarreira)!= NAO_ENCONTRADO){
        if ((*paragem)->numCarreiras > 1){
            for(c = (*paragem)->head; c->next != NULL; c = c->next){               
                if ((*paragem)->head->idcarreira == (*carreira)->idcarreira){
                    remover = c;
                    (*paragem)->head = c->next;
                    if ((*paragem)->tail->idcarreira == c->idcarreira)
                        (*paragem)->tail = NULL;
                    break;
                }
                else if (c->next != NULL && c->next->idcarreira == (*carreira)->idcarreira){
                    remover = c->next;
                    c->next = remover->next;
                    if ((*paragem)->tail->idcarreira == remover->idcarreira)
                        (*paragem)->tail = c;
                    break;
                }
            }
        }
        else{
            remover = (*paragem)->head;
            (*paragem)->head = NULL;(*paragem)->tail = NULL;
        }
        /* como as paragens têm 'copias' do id e do nome das carreiras temos de libertar esta memoria aqui */
        freeParagem(remover,paragem);
    }
}

/* reestrutura a lista de carreiras (elimina a carreira da lista) */
void ReestruturaListaCarreiras(list_carreira **lc,Carreira**c){

    Carreira * temp;

    /* mais de um elemento */
    if ((*lc)->head->next != NULL){

        /* caso de nao ser a cabeça */
        if ((*lc)->head->idcarreira != (*c)->idcarreira){
            /* procura o elemento cujo o seu sucessor seja aquele que queremos remover */
            for (temp = (*lc)->head; temp->next->idcarreira != (*c)->idcarreira ; temp = temp->next);

            if (temp->next->next == NULL)
                (*lc)->last = temp;
            temp->next = temp->next->next;
        }
        /* no caso de ser a cabeça */
        else {
            (*lc)->head = (*lc)->head->next;
        } 
    }
    else{
        (*lc)->head = NULL;
        (*lc)->last = NULL;
    }
}

/* le o nome da carreira a remover */
void comandosRemove(char **nomeCarreira){
    leEspacos();
    leNome(nomeCarreira);
    leAteFinalLinha();
}

/* free das ligações todas da carreira */
void freeLigCarreira(Carreira**c){

    Ligacao *l = (*c)-> head,*auxL;

    while (l!= NULL){
        auxL = l;
        l = l->next;
        free(auxL);
    } 
}

/* Função para tratar do comando 'r' */
void removeCarreira(list_carreira **lc,list_paragens **lp){

    char *nomeCarreira;
    int idpO,idpD;

    Carreira *c;
    Ligacao *l;
    Paragem *paragemO,*paragemD;

    comandosRemove(&nomeCarreira);
    c = encontraCarreira(lc,nomeCarreira);
    if (c == NULL){
        printf("%s: no such line.\n",nomeCarreira);
        return;
    }
    /* encontrada a carreira vamos remove-la da lista de carreiras */
    ReestruturaListaCarreiras(lc,&c);
    l = c->head;

    /* objetivo é percorrer todas as ligações da carreira e dar free de tudo */
    if (l != NULL){
        while (l!= NULL){
            idpO = l->idOrigem;
            paragemO = encontraParagemInd((*lp),idpO);
            removeCarreiraParagem(&c,&paragemO); /* remover a carreira da paragem */
            l = l->next;
        }
        idpD = c->tail->idDestino;
        paragemD = encontraParagemInd((*lp),idpD);
        removeCarreiraParagem(&c,&paragemD);
    }
    freeLigCarreira(&c);
    free(nomeCarreira);
    free(c->nome);
    free(c);
}
