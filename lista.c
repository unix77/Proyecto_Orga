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
	tPosicion nuevaCelda = (tPosicion) malloc(sizeof(struct celda));
	if(nuevaCelda == NULL){
		exit(LST_ERROR_MEMORIA);
	}
	nuevaCelda->elemento = e;
	nuevaCelda->siguiente = p->siguiente;
	p->siguiente = nuevaCelda;
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
    tPosicion aux = (*l);//lo apunto a la primer posicion (centinela)

    if(aux->siguiente == NULL){
            free((*l));     //libero la memoria donde esta la celda centinela en memoria
            (*l) = NULL;    //seteo el puntero a centinela, en NULL, porque ya no hay nada
            printf("Fin de cb destruir\n");
    }
    else{
        printf("Inicio del caso recursivo de destruir\n");
        fEliminar(aux->siguiente->elemento);
        (*l)->siguiente = aux->siguiente->siguiente;
        free(aux->siguiente);
        aux->siguiente = NULL; // por formalidad
        l_destruir(l, fEliminar);
        printf("Fin del caso recursivo de destruir\n");
    }
    printf("Bye\n");
}
