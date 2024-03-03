#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

/* criação de listas de estruturas com apontadores para cabeça e cauda */

list_carreira* list_build_c(){

    list_carreira *l = (list_carreira*)malloc(sizeof(list_carreira));
    verificaMalloc(l);
    l ->head = NULL;
    l ->last = NULL;
    return l;
}

list_paragens* list_build_p(){

    list_paragens *l = (list_paragens*)malloc(sizeof(list_paragens));
    verificaMalloc(l);
    l ->head = NULL;
    l ->last = NULL;
    return l;
}
