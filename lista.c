#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/**
 Inicializa una lista vac�a.
 Una referencia a la lista creada es referenciada en *L.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
 -----------------------------------
 Recibe puntero a puntero tLista, donde el tLista que apunta a una celda;
 (l(pointer) ->tLista(pointer)->celda(struct))
 por lo tanto se debe reservar memoria para el struct al que apunta el puntero "tLista".
 Consecuentemente, a lo que apunta (*l)(que es el puntero tLista) necesita
 una referencia a un espacio de memoria dinamica, donde se va a almacenar un stect celda, que es la centinela.

 Si a lo que apunta el puntero (l), es NULL, el programa finaliza indicando LST_ERROR_MEMORIA
 En caso contrario, se inicializan: el elemento y el siguiente de la celda centinela, en NULL.
**/
extern void crear_lista(tLista * l){
   (*l) = (tLista) malloc(sizeof(struct celda));
    if(l == NULL){                                  /*no seria (*l)==NULL??*/
        exit(LST_ERROR_MEMORIA);
    }
    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

/**
 Inserta el elemento E, en la posici�n P, en L.
 Con L = A,B,C,D y la posici�n P direccionando C, luego:
 L' = A,B,E,C,D.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
 ---------------------------------------------------------------------------------------------
 Creo un puntero pos y lo hago apuntar al espacio reservado memoria dinamica para una celda con elemento inicializado.

**/
extern void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion nueva_celda = (tPosicion)malloc(sizeof(struct celda));
    if(nueva_celda == NULL){
        exit(LST_ERROR_MEMORIA);
    }
    nueva_celda->elemento = e;
    nueva_celda->siguiente = p->siguiente;
    p->siguiente = nueva_celda;
}

/**
 Elimina la celda P de L.
 El elemento almacenado en la posici�n P es eliminado mediante la funci�n fEliminar.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
 ---------------------------------------------------------------------
 hace free de la celda
**/
extern void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    if(p->siguiente == NULL){
        exit(LST_POSICION_INVALIDA);
    }
    tPosicion celda_a_eliminar = p->siguiente;//por formalidad, voy a guardar una referencia a la celda que vamos a eliminar
    fEliminar(celda_a_eliminar->elemento);// le estoy pasando el puntero al elemento, por eso no tengo que poner &()
    //ahora, tengo que enlazar las celdas previas, y las que le siguen a la celda que quiero eliminar
    p->siguiente = celda_a_eliminar->siguiente;
    celda_a_eliminar->siguiente = NULL; // por formalidad, desvinculo el enlace
    free(celda_a_eliminar);// libero la porcion de memoria dinamica a la que apunta el puntero "celda_a_eliminar"
}

/**
 Destruye la lista L, elimininando cada una de sus celdas.
 Los elementos almacenados en las celdas son eliminados mediante la funci�n fEliminar.
 ------------------------------------------------------------------------------------
 Creo una posicion auxiliar, y la hago apuntar a la celda centinela.
 (CASO BASE)Si la lista esta vacia (el siguiente de la centinela es NULL), tengo que borrar
 la celda centinela de la memoria dinamica: hago fEliminar del elemento de la centinela(ASK), hago free de la
 celda centinela, y luego apunto (*l) a NULL para que el puntero a celda no quede apuntando a "cualquier lado".

 (CASO RECURSIVO):Si la lista no esta vacia, elimino la primer celda que le sigue a la celda centinela.
 Primero libero el espacio de memoria que ocupa el elemento con "fEliminar"
 Luego hago el enlace entre la centinela, y la celda que le sigue a la celda que estoy eliminando.
 Rompo el enlace entre la celda que estoy eliminando y la celda siguiente(la cual tomara su lugar), o NULL
 (en caso de que estemos eliminando la celda fin). Hago este paso por formalidad de codigo(no es necesario)
 Libero la memoria dinamica que ocupa la celda que quiero eliminar.
 Llamo recursivamente con el puntero al puntero tLista, y fEliminar. En este momento la lista ya tiene una
 celda menos que antes.


 Acotacion: borrar los "printf", son de ayuda para seguir el codigo
**/
extern void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    printf("Inicio de destruir\n");
    tLista aux = (*l);//lo apunto a la primer posicion (centinela)
    if(aux->siguiente == NULL){
            fEliminar((*l)->elemento); // (es un free sobre un NULL, es como no hacer nada)
            free((*l));     //libero la memoria donde esta la celda centinela en memoria
            (*l) = NULL;    //seteo el puntero a centinela, en NULL, porque ya no hay nada
            l = NULL;       //seteo el puntero que apunta a una puntero que apunta a una celda, en NULL
            printf("Fin de cb destruir\n");
    }
    else{
        printf("Inicio del caso recursivo de destruir\n");
        fEliminar(aux->siguiente->elemento);
        (*l)->siguiente = aux->siguiente->siguiente;
        aux->siguiente = NULL; // por formalidad
        free(aux->siguiente);
        l_destruir(l, fEliminar);
        printf("Fin del caso recursivo de destruir\n");
    }
    printf("Bye\n");
}

 /**
 Recupera y retorna el elemento en la posici�n P.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
extern tElemento l_recuperar(tLista l, tPosicion p){
    if(p->siguiente == NULL){//estoy en la posicion fin
        exit(LST_POSICION_INVALIDA);
    }
    return (p->siguiente->elemento);//retorno un puntero al elemento de la celda siguiente
}
/**
 Recupera y retorna la primera posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
 -------------------------------------------------
 Retorno la celda centinela, en todos los casos.
**/
extern tPosicion l_primera(tLista l){
    return l;
}
/**
 Recupera y retorna la posici�n siguiente a P en L.
 Finaliza indicando LST_NO_EXISTE_SIGUIENTE si P es fin(L).
 --------------------------------------------------------------
 Creo una posicion y la hago apuntar al centinela.
 Si el puntero p apunta a una celda que es fin, retorno el codigo de salida (porque fin es el centinela, y no tiene next.


**/
extern tPosicion l_siguiente(tLista l, tPosicion p){
    if(p->siguiente == NULL){
        exit(LST_NO_EXISTE_SIGUIENTE);
    }
    return p->siguiente;
}

/**
 Recupera y retorna la posici�n anterior a P en L.
 Finaliza indicando LST_NO_EXISTE_ANTERIOR si P es primera(L).
**/
extern tPosicion l_anterior(tLista l, tPosicion p){
    if(p == l){
        exit(LST_NO_EXISTE_ANTERIOR);
    }
    tPosicion pos = l;
    while(pos->siguiente != p){
        pos = pos->siguiente;
    }
    return pos;
}

/**
 Recupera y retorna la �ltima posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
 ------------------------------------------------------
 Caso 1) La lista esta vacia: en ese caso ultima es el centinela.

 Caso 2) La lista tiene un solo elemento: en ese caso "ultima" es el centinela

 Caso 3) La lista tiene + de 1 elemento: primera es el centinela, fin es la ultima celda, y ultima la anterior a fin
**/
extern tPosicion l_ultima(tLista l){
    tPosicion pos = l;
    if(pos->siguiente == NULL || pos->siguiente->siguiente == NULL ){
        return pos;
    }
    //se que tiene mas de 1 elemento, inicializo los punteros tal que pueda llegar a una situacion (p->celda -> (celda (<-pfin))->null)
    tPosicion pfin = pos->siguiente;
    while(pfin->siguiente != NULL){
        pos = pos->siguiente;
        pfin = pfin->siguiente;
    }
    return pos;
}
/**
 Recupera y retorna la posici�n fin de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
 ----------------------------------------------------------------
 Inicializo un puntero que apunte a la celda centinela. Mientras su siguiente no sea NULL, avanzo el puntero.
**/
extern tPosicion l_fin(tLista l){
    tPosicion pos = l;
    while(pos->siguiente != NULL){
        pos = pos->siguiente;
    }
    return pos;
}

/**
 Retorna la longitud actual de la lista.
**/
extern int l_longitud(tLista l){
    int longitud = 0;
    tPosicion pos = l;
    while (pos->siguiente != NULL){
        longitud++;
        pos = pos->siguiente;
    }
    return longitud;
}


