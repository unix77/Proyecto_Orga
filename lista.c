#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void crear_lista(tLista* l){
	*l = (tLista) malloc(sizeof(struct celda));
	if(l == NULL){
		exit(LST_ERROR_MEMORIA);
	}
	(*l)->elemento = NULL;
	(*l)->siguiente = NULL;
}


tPosicion l_primera(tLista l){
	return l;
}

void l_insertar(tLista l, tPosicion p, tElemento e){
	tPosicion nueva_celda = (tPosicion) malloc(sizeof(struct celda));
	if(nueva_celda == NULL){
		exit(LST_ERROR_MEMORIA);
	}
	nueva_celda->elemento = e;
	nueva_celda->siguiente = p->siguiente;
	p->siguiente = nueva_celda;
}

tElemento l_recuperar(tLista l, tPosicion p){
	if(p->siguiente == NULL){
		exit(LST_POSICION_INVALIDA);
	}
	return p->siguiente->elemento;
}


tPosicion l_siguiente(tLista l, tPosicion p){
	if(p->siguiente == NULL){
		exit(LST_NO_EXISTE_SIGUIENTE);
	}
	return p->siguiente;
}


tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion pos;
    if (p == l){
        exit (LST_NO_EXISTE_ANTERIOR);
    } else{
        pos = l->siguiente;
        while (pos->siguiente != p){
            pos = pos->siguiente;
        }
    }
    return pos;
}

tPosicion l_ultima(tLista l){
    tPosicion pos;
   if (l->siguiente != NULL){
        pos = l->siguiente;
       if (pos->siguiente != NULL){
            while (pos->siguiente->siguiente != NULL){
                pos = pos->siguiente;
            }
       }
   }
    return pos;
}

tPosicion l_fin(tLista l){
    tPosicion pos;
    if (l->siguiente != NULL){
        pos = l->siguiente;
        while (pos->siguiente != NULL){
            pos = pos->siguiente;
        }
    }
    return pos;
}

int l_longitud(tLista l){
    tPosicion pos = l;
    int longitud;
    if (pos->siguiente == NULL){
        longitud = 0;
    } else {
      	longitud = 1;
        pos = pos->siguiente;
        while (pos->siguiente != NULL){
            longitud++;
            pos = pos ->siguiente;
        }
    }
    return longitud;
}

void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    if(p->siguiente == NULL){
        exit(LST_POSICION_INVALIDA);
    }
    //Por formalidad se guarda una referencia a la celda que se eliminará
    tPosicion celda_a_eliminar = p->siguiente;
    fEliminar(celda_a_eliminar->elemento);
    p->siguiente = celda_a_eliminar->siguiente;
    celda_a_eliminar->siguiente = NULL; //Por formalidad, se desvincula el enlace
    free(celda_a_eliminar);
}

/**
 Destruye la lista L, elimininando cada una de sus celdas.
 Los elementos almacenados en las celdas son eliminados mediante la funci�n fEliminar.
 ------------------------------------------------------------------------------------
 Definiendo a L como la lista con todas sus posiciones y a L' como la lista sin la primer posición.

 (CASO BASE): Si la lista esta vacia se elimina la celda centinela.
 (CASO RECURSIVO): Si la lista no está vacía se elimina la primer posición y realiza l_destruir para L'.

**/
void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    tPosicion aux = (*l);

    if(aux->siguiente == NULL){
            free((*l));
            (*l) = NULL;
    }
    else{
        fEliminar(aux->siguiente->elemento);
        (*l)->siguiente = aux->siguiente->siguiente;
        free(aux->siguiente);
        aux->siguiente = NULL;
        l_destruir(l, fEliminar);
    }
}
